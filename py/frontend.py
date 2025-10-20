from PyQt6.QtWidgets import QWidget

class Frontend(QWidget):
    def __init__(self):
        super().__init__()

        # Window initialization
        self.setFixedSize(1000,700)
        self.setWindowTitle('seat allocator remake')


