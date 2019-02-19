基于UDP的网络网络通信模型：
Server:socket() -> bind() -> recvfrom()/sendto() -> close()
Client:socket() -> connect() -> sendto()/recvfrom() -> close()
