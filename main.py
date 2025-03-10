import sys
import socket
from PyQt5.QtWidgets import QApplication, QWidget, QVBoxLayout, QPushButton, QLabel, QFileDialog, QMessageBox
from PyQt5.QtGui import QPixmap
from python.image_processor import process_image

class ImageUploader(QWidget):
    def __init__(self):
        super().__init__()
        self.initUI()

    def initUI(self):
        self.setWindowTitle('Image Uploader')
        self.setGeometry(100, 100, 800, 600)

        self.layout = QVBoxLayout()

        self.uploadButton = QPushButton('Upload Image', self)
        self.uploadButton.clicked.connect(self.upload_image)
        self.layout.addWidget(self.uploadButton)

        self.imageLabel = QLabel(self)
        self.layout.addWidget(self.imageLabel)

        self.setLayout(self.layout)

    def upload_image(self):
        options = QFileDialog.Options()
        fileName, _ = QFileDialog.getOpenFileName(self, "Select Image to Upload", "", "Images (*.png *.jpg *.jpeg *.bmp)", options=options)
        if fileName:
            self.send_image(fileName)

    def send_image(self, image_path):
        server_address = ('127.0.0.1', 8080)

        try:
            with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as sock:
                sock.connect(server_address)
                sock.sendall(image_path.encode('utf-8'))

                retries = 3
                while retries > 0:
                    try:
                        data = sock.recv(1024)
                        if data:
                            self.display_image(data.decode('utf-8'))
                            break
                    except ConnectionResetError as e:
                        print(f"Connection was reset: {e}, retrying...")
                        retries -= 1
                        if retries == 0:
                            QMessageBox.critical(self, "Error", "Failed to receive data after multiple attempts.")
                            return
        except Exception as e:
            QMessageBox.critical(self, "Error", f"An error occurred: {e}")

    def display_image(self, image_path):
        pixmap = QPixmap(image_path)
        self.imageLabel.setPixmap(pixmap)
        self.imageLabel.setScaledContents(True)

if __name__ == "__main__":
    app = QApplication(sys.argv)
    ex = ImageUploader()
    ex.show()
    sys.exit(app.exec_())