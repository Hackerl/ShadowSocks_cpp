# ShadowSocks_cpp
## 编译
```shell=
apt: sudo apt install cmake pkg-config libgoogle-glog-dev libssl-dev libevent-dev libjsoncpp-dev
pacman: sudo pacman -S cmake pkgconf google-glog openssl libevent jsoncpp
```
```shell=
git clone https://github.com/Hackerl/ShadowSocks_cpp
cd ShadowSocks_cpp
mkdir build
cd build
cmake ..
make
```
```shell=
> ShadowSocksr --help                                
usage: ShadowSocksr [options] ... 
options:
  -c, --config    Config File Path (string [=Config.json])
  -?, --help      print this message
```
## 文件
```=
├── Config.json                    配置文件
├── libEventLoop.so
├── libSocketNode.so
├── libSocket.so
├── plugin
│   ├── libPortTunnel.so
│   ├── libSocketConnector.so
│   ├── libSocksClient.so
│   ├── libSocksServer.so
│   ├── libSSRLocal.so
│   └── SSR
│       ├── libAuthChain.so
│       ├── libHTTPSimple.so
│       └── libTLSTicketAuth.so
├── Protocol.json                  SSR配置文件
└── ShadowSocksr

```

## 设计
端口转发、本地Socks5代理、内网穿透这些功能的模型都是类似的，由一个本地端口与远程端口组成的数据流，直接转发本地与远程的数据，或者进行一些加密解密处理。
### INode接口
一个数据链由若干节点组成:
```c=
class INodeClose : public Interface
{
public:
    virtual void NodeClose() = 0;
};

class INodeCallback : public INodeClose
{
public:
    virtual bool OnUpStream(const void *Buffer, size_t Length) = 0;
    virtual bool OnDownStream(const void *Buffer, size_t Length) = 0;
};

class INode : public INodeCallback
{
public:
    virtual bool OnInitManager(std::shared_ptr<INodeManager> NodeManager) = 0;
    virtual bool UpStream(const void *Buffer, size_t Length) = 0;
    virtual bool DownStream(const void *Buffer, size_t Length) = 0;
    virtual void SetUpNode(INodeCallback * UpNode) = 0;
    virtual void SetDownNode(INodeCallback * DownNode) = 0;
};
```
为节点设置上行节点与下行节点，然后调用UpStream将处理之后的数据传递给上行节点。上行节点OnUpStream函数用于回调接收数据。当然其中任何节点都可以觉得是否继续数据上行，例如Socks5代理节点会先与客户端握手，这个过程是该节点与客户端的数据传输，握手成功之后Socks5代理节点状态变为已连接，之后的数据都是直接透明上行。
### 图解
![](https://i.imgur.com/DF6yDQh.png)
### 节点管理
节点除了上行下行数据流之外，还需要一个负责管理的类。例如客户端接收数据不及时，本地端口节点Socket写操作便会失败，因为内核缓冲区已满。但此时远程端口节点没有任何改变，有读事件时会立刻读数据然后下行。所以需要一个可以发布事件的机制，引入INodeManager接口。
```c=
class INodeEventCallback : public Interface
{
public:
    virtual void OnNodeEvent(NodeEventRegister EventID, void *Context) = 0;
};

class INodeServiceCallback : public Interface
{
public:
    virtual bool OnNodeService(NodeServiceRegister ServiceID, void *Context) = 0;
};

class INodeManager : public Interface
{
public:
    virtual void RegisterEvent(NodeEventRegister EventID, INodeEventCallback *Node) = 0;
    virtual void BroadcastEvent(NodeEventRegister EventID, void *Context, INodeEventCallback *Publisher) = 0;
    virtual bool RegisterService(NodeServiceRegister ServiceID, INodeServiceCallback *Node) = 0;
    virtual bool InvokeService(NodeServiceRegister ServiceID, void *Context) = 0;
};
```
节点可以注册事件回调，也可以发布事件。这样本地端口阻塞时发布事件，远程端口节点Socket停止监听读事件。当本地端口节点Socket写完数据之后，发布流通事件。
另外还需要服务机制，远程端口节点的Socket是不确定的，而本地端口节点在监听端口Accpet之后就立即设置的。
Socks5等代理服务也是在与客户端握手之后，客户端发送目的IP与Port，此时才能进行Socket连接并设置给远程端口。
所以建立服务机制，远程端口节点提供INIT_REMOTE_SOCKET服务，当Socks5插件节点握手成功之后，连接服务器，然后调用INIT_REMOTE_SOCKET服务用于设置远程端口。
### 节点关闭
如果有节点需要关闭，该数据链条需要一同关闭。所以NodeManager管理类提供REQUEST_CLOSE_PIPE服务，当某个节点发生错误，或则端口节点收到套接字关闭的事件时，可以调用该服务。
## 插件
插件接口：
```c=
class IPlugin : public Interface
{
public:
    virtual bool InitPlugin(const void Context, size_t Size) = 0;
};
```
插件借鉴了COM接口思想，纯虚函数表作为接口，然后每个动态库通过导出函数创建一个实例。
libPortTunnel.so动态库导出函数：
```c=
extern "C" IPlugin * NewPlugin()
{
    return new CPortTunnel;
}
```
上面是一个端口转发插件，通过配置文件传递目标IP、端口：
```javascript=
{
	"Server" : "127.0.0.1",
	"Port" : 1081,
	"Nodes":
	[
		{
			"Name" : "plugin/libPortTunnel.so",
			"Config" :
			{
				"TargetIP" : "127.0.0.1",
				"TargetPort" : 2222
			}
		}
	],
	"Services":
        [
            {
                "Name" : "plugin/libSocketConnector.so",
            }
        ]
}
```
### libSocketConnector
为了避免让每一个Node自己连接远程端口，提供libSocketConnector插件。libSocketConnector提供REQUEST_SOCKET_CONNECT服务，传递IP(域名)、Port给插件，插件会自动连接并设置远程端口节点。
libSocketConnector还可以设置代理，现在只实现了HTTPTunnel代理。如果你在公司内网，是无法直接访问外网的，一般内网会提供HTTP代理服务器。
所以可以这样使用端口转发：
```javascript=
{
	"Server" : "127.0.0.1",
	"Port" : 1081,
	"Nodes":
	[
		{
			"Name" : "plugin/libPortTunnel.so",
			"Config" :
			{
				"TargetIP" : "www.baidu.com",
				"TargetPort" : 80
			}
		}
	],
    "Services":
    [
        {
            "Name" : "plugin/libSocketConnector.so",
            "Config" :
            {
                "ProxyType" : "HTTPTunnel",
                "ProxyServer" : "127.0.0.1",
                "ProxyPort" : 12759
            }
        }
    ]
}
```
## SSR
我开始的目的只是想穿透公司内网，不被公司监控流量。各种支持HTTP代理的穿透工具都可以，但是因为我使用ShadowSocksr比较多，而且购买了服务，所以想自己实现一个工具。然而SSR不支持HTTP代理，然后一步步探索，有了现在这个小框架。
SSR也被写成了插件形式，但是我只实现了我常用的几个协议，而且只是客户端加密解密接口。全部实现可能较为繁琐，而且对我的帮助不大，所以暂且搁置。
SSR统一协议接口：
```c=
class IProtocol : public Interface
{
public:
    virtual u_short GetOverHead() = 0;
    virtual void SetServerInfo(CServerInfo & Info) = 0;

public:
    virtual bool NeedSendBack() = 0;
    virtual std::vector<u_char> PackSendBackData() = 0;

public:
    virtual std::vector<u_char> ClientPack(const u_char *Buffer, size_t Length) = 0;
    virtual std::vector<u_char> ClientUnPack(const u_char *Buffer, size_t Length) = 0;
    virtual std::vector<u_char> ServerPack(const u_char *Buffer, size_t Length) = 0;
    virtual std::vector<u_char> ServerUnPack(const u_char *Buffer, size_t Length) = 0;
};
```
不再区分协议、加密、混淆，都抽象为协议接口。
已实现协议：
* http_simple
* auth_chain_a
* tls1.2_ticket_auth
### 配置
需要将协议映射到动态库，因为一个动态库可能实现多个类似协议，例如auth_chain_a，auth_chain_b都在libAuthChains.so中。
通过导出函数创建协议实例：
```c=
extern "C" IProtocol * NewProtocol(const char * ProtocolName)
{
    //可拓展为多个协议 这里只是auth_chain_a
    if (std::string(ProtocolName) != "auth_chain_a")
        return nullptr;

    return new CAuthChainLocal;
}
```
在Application目录下，编写Protocol.json文件：
```javascript=
{
    "http_simple" : "plugin/SSR/libHTTPSimple.so",
    "auth_chain_a" : "plugin/SSR/libAuthChain.so",
    "tls1.2_ticket_auth" : "plugin/SSR/libTLSTicketAuth.so"
}
```
没有这个配置文件进行映射是无法工作的。
## 配置示例
### 端口转发
libSocketConnector可以设置代理，用于网络受限的情况。
```javascript=
{
	"Server" : "127.0.0.1",
	"Port" : 1081,
	"Nodes":
	[
		{
			"Name" : "plugin/libPortTunnel.so",
			"Config" :
			{
				"TargetIP" : "www.baidu.com",
				"TargetPort" : 80
			}
		}
	],
    "Services":
    [
        {
            "Name" : "plugin/libSocketConnector.so",
        }
    ]
}
```
### 本地Socks5代理
```javascript=
{
	"Server" : "127.0.0.1",
	"Port" : 1081,
	"Nodes":
	[
		{
			"Name" : "plugin/libSocksClient.so"
		},
		{
			"Name" : "plugin/libSocksServer.so"
		}
	],
    "Services":
    [
        {
            "Name" : "plugin/libSocketConnector.so",
        }
    ]
}
```
### SSR客户端
```javascript=
{
    "Server" : "127.0.0.1",
    "Port" : 1080,
    "Nodes":
    [
        {
            "Name" : "plugin/libSocksClient.so"
        },
        {
            "Name" : "plugin/libSSRLocal.so",
            "Config" : 
            {
                "Server" : "***.com",
                "Port" : 1080,
                "PassWord" : "123456",
                "Protocols" : ["auth_chain_a", "tls1.2_ticket_auth"]
            }
        }
    ]
    "Services":
    [
        {
            "Name" : "plugin/libSocketConnector.so",
        }
    ]
}
```
还需配置Protocol.json文件
```javascript=
{
    "http_simple" : "plugin/SSR/libHTTPSimple.so",
    "auth_chain_a" : "plugin/SSR/libAuthChain.so",
    "tls1.2_ticket_auth" : "plugin/SSR/libTLSTicketAuth.so"
}
```
