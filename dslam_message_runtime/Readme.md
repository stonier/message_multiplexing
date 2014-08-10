## PubSub (De)MultiPlexers

These classes allow a regular nanomsg pubsub system to be mux'd on the publisher side and demux'd
on the subscriber side. On most systems you wouldn't worry about it (c.f. ros which
creates a port for every pub/sub connection), however for embedded systems this gets expensive -
the extra creation of ports does get somewhat costly.

Operation:

* Use a packet id (unsigned int) for each type of publication.
* Decoupled marshalling code in dslam::Message<T> classes.
* Mux and demux registered with a name/url pair. Pubs and subs connect by name.
* Publishers can publish from anywhere.
* Only one subscriber per packet id


Todo:

Must dos:

* too many to list 

Maybe dos:

* Relax the one subscriber per packet id constraint.
