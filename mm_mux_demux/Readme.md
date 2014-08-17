## PubSub (De)MultiPlexers

These classes allow a regular nanomsg pubsub system to be mux'd on the publisher side and demux'd
on the subscriber side. On most systems you wouldn't worry about it (c.f. ros which
creates a port for every pub/sub connection), however for embedded systems this gets expensive -
the extra creation of ports does get somewhat costly.

### Overview

* Use a packet id (unsigned int) for each type of publication.
* Decoupled marshalling code in message_multiplexing::Message<T> classes.
* Mux and demux registered with a name/url pair. Pubs and subs connect by name.
* Publishers can publish from anywhere.
* Only one subscriber per packet id

### Classes

* Message : basic serialiser/deserialiser for a particular message type
* Publisher : can publish any/all packet types
* Subscriber : single packet type receiver which will convert a received buffer and relay to a user callback
* MessageRegistry : global enabler for managing the mux/demux's and their connection with pubs/subs.

### Roadmap

Todos:

* zero-copy buffer allocations via nn_allocmsg for nanomsg transport
 * as reduce cost of resizing buffers on the way in via the publisher (spec. for images, maybe c++11 std::move?)
* transports - mux and demux should separate the actual underlying transport (nanomsg/serial...)
* handle use case in which packets are gathered and sent (e.g. kobuki style)
 * optional flag when sending that belays the send.
 * ~~split packet signature from packet id/payload length~~
* ~~loadable { packet_id : data_type } registries for pubs and subs to check against~~
* ~~switch to ecl byte array converters in `impl/builders.hpp`~~
* ~~mux should create muxes on the fly like demux~~
 * ~~publisher to encode, then send a buffer to the mux which relays across the wire ~~
* ~~convert the builders to specific Message class types with encode/decode functionality~~

Maybe dos:

* Relax the one subscriber per packet id constraint.

### Testing

You can test the subscriber end of a connection without programming using `nanocat`. An example:

```
> nanocat --sub --connect tcp://192.168.1.3:5555 --ascii
```
