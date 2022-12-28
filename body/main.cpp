/*
 * Copyright (c) 2022-2023 David Snowdon
 * Licensed under  GNU General Public License, Version 3 (the "License");
 * You may not use this file except in compliance with the License.
 * A copy of the license is contained in the file LICENSE in the root of this repo.
 */
#include <chrono>
#include <iostream>
#include <memory>
#include <random>
#include <string>
#include <thread>

#include <grpc/grpc.h>
#include <grpcpp/channel.h>
#include <grpcpp/client_context.h>
#include <grpcpp/create_channel.h>
#include <grpcpp/security/credentials.h>
#include "thesocialrobot.grpc.pb.h"

using grpc::Channel;
using grpc::ClientContext;
using grpc::ClientReader;
using grpc::ClientReaderWriter;
using grpc::ClientWriter;
using grpc::Status;
using thesocialrobot::TheSocialRobot;
using thesocialrobot::BodyEvent;
using thesocialrobot::BodyCommand;

BodyEvent MakeBodyEvent(const int id) {
    BodyEvent e;
    e.set_id(id);
    return e;
}

class TheSocialRobotClient {
public:
    TheSocialRobotClient(std::shared_ptr <Channel> channel)
            : stub_(TheSocialRobot::NewStub(channel)) {
    }

    void EventStream() {
        ClientContext context;

        std::shared_ptr <ClientReaderWriter<BodyEvent, BodyCommand>> stream(
                stub_->EventStream(&context));

        std::thread writer([stream]() {
            std::vector <BodyEvent> events{
                    MakeBodyEvent(42)
            };
            for (const BodyEvent &event: events) {
                std::cout << "Sending event " << event.id() << std::endl;
                stream->Write(event);
            }
            stream->WritesDone();
        });

        BodyCommand command;
        while (stream->Read(&command)) {
            std::cout << "Got message " << command.id()  << std::endl;
        }
        writer.join();
        Status status = stream->Finish();
        if (!status.ok()) {
            std::cout << "EventStream rpc failed." << std::endl;
        }
    }

private:
    std::unique_ptr <TheSocialRobot::Stub> stub_;
};

int main(int argc, char **argv) {
    // TODO: parse command line arguments
    TheSocialRobotClient client(
            grpc::CreateChannel("localhost:50051",
                                grpc::InsecureChannelCredentials()));

    client.EventStream();

    return 0;
}
