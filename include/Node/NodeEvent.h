//
// Created by patteliu on 2018/11/12.
//

#ifndef SHADOWSOCKSR_CPP_NODEEVENT_H
#define SHADOWSOCKSR_CPP_NODEEVENT_H

enum NodeEventRegister : unsigned int
{
    PIPE_NODE_BLOCK = 0,
    PIPE_STREAM_BLOCK,
    PIPE_STREAM_FLOW
};

#endif //SHADOWSOCKSR_CPP_NODEEVENT_H
