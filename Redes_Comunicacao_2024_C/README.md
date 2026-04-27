# News Service <img src="https://cdn.jsdelivr.net/gh/devicons/devicon/icons/c/c-original.svg" width="60"/>

Communication Networks

## Description

Distributed system for content dissemination using TCP, UDP, and IP multicast. Users are organized in classes where messages are shared in real time.

## Architecture

- **Server:** Authentication, user and class management, multicast coordination
- **Admin Console:** UDP interface to manage users (CLI)
- **Client:** TCP connection, allows students and professors to interact with classes

## Functionalities

- User authentication (admin, student, professor)
- Class creation and subscription
- Multicast communication within classes
- User management via admin console
- Support for multiple clients

## Communication

- TCP → client-server
- UDP → admin-server
- Multicast → class messages

## Network

- 3 routers + 2 switches (GNS3)
- NAT for private network 
- Public addressing
