/*
Smooth - A C++ framework for embedded programming on top of Espressif's ESP-IDF
Copyright 2019 Per Malmberg (https://gitbub.com/PerMalmberg)

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#include "smooth/application/network/mqtt/packet/PubRec.h"
#include "smooth/application/network/mqtt/packet/IPacketReceiver.h"

namespace smooth::application::network::mqtt::packet
{
    void PubRec::visit(IPacketReceiver& receiver)
    {
        receiver.receive(*this);
    }

    uint16_t PubRec::get_packet_identifier() const
    {
        calculate_remaining_length_and_variable_header_offset();

        return read_packet_identifier(get_variable_header_start());
    }
}
