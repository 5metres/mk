
import socket
from python.image_processor import process_image
def main():
    server_address = ('127.0.0.1', 8080)
    image_path = "C:/Users/admin/Desktop/2222222.png"  # 请确保该图片文件存在于服务器可访问的路径
    # print(process_image(image_path))
    # 创建 TCP 套接字
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as sock:
        sock.connect(server_address)
        # 发送图片文件路径
        sock.sendall(image_path.encode('utf-8'))
        # 接收处理结果（处理后图片的路径）
        retries = 3
        while retries > 0:
            try:
                data = sock.recv(1024)
                break
            except ConnectionResetError as e:
                print(f"Connection was reset: {e}, retrying...")
                retries -= 1
                if retries == 0:
                    print("Failed to receive data after multiple attempts.")
                    return


if __name__ == "__main__":
    main()
