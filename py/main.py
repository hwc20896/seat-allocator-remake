from PyQt6.QtWidgets import QApplication
import sys
from frontend import Frontend

if __name__ == '__main__':
    app = QApplication(sys.argv)
    window = Frontend()
    window.show()
    sys.exit(app.exec())