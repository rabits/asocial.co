aSocial
=======
[![Build Status](https://drone.io/github.com/rabits/asocial.co/status.png)](https://drone.io/github.com/rabits/asocial.co/latest)

OpenSource crossplatform disturbed social network.

About
-----
With the advent of high-performance personal devices, available cryptography and mesh networks - secure distributed networks are becoming an integral part of our lives. I believe that the time has come to social networks to reach the decentralized space.

Bitcoin shows that no sense to lend money to banks in possession to conveniently and safely use your money. So what is the point to provide your social information in the hands of corporations that disclose it to third parties (government, advertising agencies, etc.)? 

With the aSocial.co you can securely store social data on your devices and transfer that data only one to whom see fit - relatives, friends, anyone (or, maybe, noone).

Basic features
--------------

### Profile
Your social profile, that contains any information you need (like achievements, projects, blog posts etc...)

### Connections
Any interaction with other profiles (like personal messages, chats, likes etc.)

### Tree of life
You can organize your known profiles in tree structure to store information about the relationships.

### Line of life
Edit your history - any information is tied to a datetime region that you can edit.
Also, you can freely travel through your or any known line of line.

### Overlays
Allows you to edit data of anyone - your changes will be attached and overlap the original data.

### Bitcoin wallet
You profile ID & device ID is a bitcoin address, so you can send or receive money using this addresses.

Main objectives
---------------

### Privacy & control
Provides by encryption and full control over stored & transmitted data outside.
 * You can send anonymous, signed or encrypted information.
 * You can freely edit your information
 * With "overlays" you also can adit any information you have

### Independence
You can transmit information by any available method:
 * To a known server in the Internet, and recipient will take it.
 * To any device in you local network, which in turn pass it down the chain until it finds recipient.
 * To any device nearby, by Point-to-Point WiFi connection, bluetooth, NFC etc...
 * To any file, to use as transport your flash drive, hands and legs - and import required data on another device.

Also it is possible to run your own aSocial service in the Internet to expand the number of shared services.

### Replication
You can transmit your profile between known devices to eliminate the possibility of loss information.

Secondary objectives
--------------------
Cool trifles that would be great to see in the application.

### HTTP/HTTPS support
Small http server, that provides access to your data. Makes possible to create links for other people, that don't have asocial application and simple would like to see your profile or part of your data.

### Make the ability to transfer money to any person with whom you do not even know
Device has identifier (like profile) - it is the Bitcoin address. So, you can transfer your money to any device in network.

Spam issue
----------
I still think about this serious issue, but only one solution seems correct: it is micro-payment for the outgoing information.

Value of comission for the message consists of 4 parts:
 * Message receive - payment for receiving message by target recipient
 * Message size - payment for storage of message
 * Message transfer - anyone who relayed the message will be paid upon confirmation of delivery

It should be configurable settings to allow the existence of free postmens and storages. The receiver response will return your money back. So, free desturbed social network is possible!

Support
-------
If you like future innovations, you can support our open-source development by a small Bitcoin donation.

Bitcoin wallet: `15phQNwkVs3fXxvxzBkhuhXA2xoKikPfUy`

Developing
----------
You need to clone this git repository recursively:

```sh
git clone --recursive https://github.com/rabits/asocial.co.git
```

### Modules

This project uses separated modules to share code between frontend & backend.

* Frontend
* Backend
* Crypto
* Network
* Database
