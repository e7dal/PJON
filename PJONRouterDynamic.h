
 /*-O//\         __     __
   |-gfo\       |__| | |  | |\ | ®
   |!y°o:\      |  __| |__| | \| v10.1
   |y"s§+`\     multi-master, multi-media bus network protocol
  /so+:-..`\    Copyright 2010-2018 by Giovanni Blu Mitolo gioscarab@gmail.com
  |+/:ngr-*.`\
  |5/:%&-a3f.:;\
  \+//u/+g%{osv,,\
    \=+&/osw+olds.\\
       \:/+-.-°-:+oss\
        | |       \oy\\
        > <
 ______-| |-__________________________________________________________________

PJONRouterDynamic has been contributed by Fred Larsen.

It performs the same as PJONRouterExtended, but populates the routing table
dynamically based on observed packets from remote buses.

If you believe in this project and you appreciate our work, please, make a
donation. The PJON Foundation is entirely financed by contributions of wise
people like you and its resources are solely invested to cover the development
and maintainance costs.
- Paypal:   https://www.paypal.me/PJON
- Bitcoin:  1FupxAyDTuAMGz33PtwnhwBm4ppc7VLwpD
- Ethereum: 0xf34AEAF3B149454522019781668F9a2d1762559b
Thank you and happy tinkering!
 _____________________________________________________________________________

Copyright 2010-2018 by Giovanni Blu Mitolo gioscarab@gmail.com

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License. */

#pragma once

#define PJON_ROUTER_TABLE_SIZE 100

#include <PJONRouter.h>

class PJONRouterDynamic : public PJONRouter {
protected:

  void add_sender_to_routing_table(const PJON_Packet_Info &packet_info, uint8_t sender_bus) {
    uint8_t start_search = 0;
    uint8_t found_bus = find_bus_with_id(packet_info.sender_bus_id, 
                                         packet_info.sender_id, 
                                         start_search);
    if (found_bus == PJON_NOT_ASSIGNED) { 
      // Not found among attached buses or in routing table. Add to table.
      add(packet_info.sender_bus_id, sender_bus);
    }
  }

  virtual void dynamic_receiver_function(uint8_t *payload, uint16_t length, const PJON_Packet_Info &packet_info) {
    // Do standard routing but also add unknown remote buses to routing table
    add_sender_to_routing_table(packet_info, current_bus);
    PJONSwitch::dynamic_receiver_function(payload, length, packet_info);
  }
  
public:  
  PJONRouterDynamic() {}
  PJONRouterDynamic(uint8_t bus_count,
                     PJONAny *buses[],
                     uint8_t default_gateway = PJON_NOT_ASSIGNED) 
                     : PJONRouter(bus_count, buses, default_gateway) { }
};