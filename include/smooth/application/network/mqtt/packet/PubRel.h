//
// Created by permal on 7/22/17.
//

#pragma once

#include <smooth/application/network/mqtt/packet/MQTTPacket.h>

namespace smooth
{
    namespace application
    {
        namespace network
        {
            namespace mqtt
            {
                namespace packet
                {
                    class PubRel
                            : public MQTTPacket
                    {
                        public:
                            PubRel() = default;

                            PubRel(const MQTTPacket& packet) : MQTTPacket(packet)
                            {
                            }

                            PubRel(uint16_t packet_id)
                            {
                                // Set fixed header
                                set_header(PUBREL, 0x2);

                                std::vector<uint8_t> variable_header;
                                append_msb_lsb(packet_id, packet);
                                calculate_remaining_length_and_variable_header_offset();

                            }

                            void visit( IPacketReceiver& receiver ) override;

                            bool has_packet_identifier() const override
                            {
                                return true;
                            }
                    };
                }
            }
        }
    }
}
