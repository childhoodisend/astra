import socket

def main():
    print('\n-----START-----')

    send_msg = "356a192b7913b04c54574d18c28d46e6395428a1"

    sock = socket.socket()
    try:
        sock.connect(("127.0.0.1", 4000))
    except ConnectionRefusedError:
        raise ConnectionRefusedError

    sock.send(send_msg.encode())
    print("\nSEND    {}".format(send_msg))

    receive_msg = sock.recv(1024).decode()
    print("\nRECEIVE {}".format(receive_msg))

    sock.close()

if __name__ == '__main__':
    main()