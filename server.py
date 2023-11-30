from _thread import *
import socket
import random
import string
import json

HOST = "0.0.0.0"
PORT = 8080

rooms = {}

def init_server():
    server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server_socket.bind((HOST, PORT))
    server_socket.listen()
    print(">> Server Ready ...")
    return server_socket;

def start_server(server_socket):
    while True:
        client_socket, addr = server_socket.accept()
        start_new_thread(threaded, (client_socket, addr))

def generate_room_code():
    while True:
        code = ''.join(random.choices(string.ascii_uppercase + string.digits, k=5))
        if code not in rooms.values():
            return code

def delete_client_socket(client_socket, code):
    if code in rooms and client_socket in rooms[code]:
        rooms[code].remove(client_socket)

        if len(rooms[code]) == 0:
            del rooms[code]
        
        print(f">> Room state: {rooms}")

def send_msg_to_room_member(client_socket, code, msg):
    for client in rooms[code]:
        if client != client_socket:
            client.send(msg.encode())

def threaded(client_socket, addr):
    print(f">> Connected by {addr[0]}:{addr[1]}")

    info = {
        "msg": "",
        "code": "",
        "score": 0,
        "length": 0,
        "food": [],
        "location": []
    }

    while True:
        try:
            recv_msg = client_socket.recv(10240).decode()

            try:
                parsed_json = json.loads(recv_msg)
                print(f">> client: {parsed_json}")

                info["msg"] = parsed_json["msg"]
                info["score"] = parsed_json["score"]
                info["length"] = parsed_json["length"]
                info["food"] = parsed_json["food"]
                info["location"] = parsed_json["location"]

                if info["msg"] == "create":
                    code = generate_room_code()
                    rooms[code] = [client_socket]
                    
                    info["msg"] = "create"
                    info["code"] = code

                    print(f">> {addr[0]}:{addr[1]} created room {code}")
                    
                    client_socket.send(json.dumps(info).encode())

                elif info["msg"].startswith("join"):
                    code = info["msg"].split(" ")[1].upper()

                    if code != None and code in rooms and len(rooms[code]) <= 2:
                        rooms[code].append(client_socket)
                        
                        info["msg"] = "ready"
                        info["code"] = code

                        print(f">> {addr[0]}:{addr[1]} joined room {code}")
                        print(f">> Room state: {rooms}")
                        send_msg_to_room_member(client_socket, code, json.dumps(info))

                    else:
                        info["msg"] = "join error"

                    client_socket.send(json.dumps(info).encode())
                
                elif info["msg"] == "start":
                    client_socket.send(json.dumps(info).encode())
                    send_msg_to_room_member(client_socket, code, json.dumps(info))
                
                elif info["msg"] == "end":
                    client_socket.send(json.dumps(info).encode())
                    send_msg_to_room_member(client_socket, code, json.dumps(info))

                elif info["msg"] == "move":
                    send_msg_to_room_member(client_socket, code, json.dumps(info))
                    
                print(f">> server: {info}")

            except Exception as ex:
                continue
                    
        except ConnectionResetError as e:
            print(f">> Disconnected by {addr[0]}:{addr[1]}")
            print(f">> Room state: {rooms}")
            break
    
    delete_client_socket(client_socket, info["code"])
    client_socket.close()

if __name__ == "__main__":
    start_server(init_server())