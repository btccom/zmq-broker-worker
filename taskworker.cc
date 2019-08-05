#include "zmq.hpp"
#include <chrono>
#include <thread>
#include <iostream>
#include <memory>
using namespace std;

int main (void)
{
    zmq::context_t *context = new zmq::context_t(1);
    zmq::socket_t receiver(*context, ZMQ_PULL);
    receiver.connect("tcp://localhost:6666");

    zmq::socket_t sender(*context, ZMQ_PUSH);
    sender.connect("tcp://localhost:5558");
    zmq::message_t zcontent;

    while (1) {
        try {
            receiver.recv(&zcontent);
        } catch (zmq::error_t &e) {
            std::cout << " zmq recv exception: " << e.what();
            break;
        }
        const string content = std::string(static_cast<char *>(zcontent.data()), zcontent.size());
        cout << "type : " << content << endl;
        // std::this_thread::sleep_for(std::chrono::milliseconds(1000)); 
        string message("task finished!");
        sender.send (message.c_str(), message.size(), 0);
    }
    receiver.close();
    sender.close();
    return 0;
}