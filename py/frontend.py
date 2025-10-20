from PyQt6.QtWidgets import QWidget
from ui.ui_widget import Ui_Widget

class Frontend(QWidget, Ui_Widget):
    def __init__(self):
        super().__init__()
        self.setupUi(self)

        # Window initialization
        self.setFixedSize(1000,750)
        self.setWindowTitle('seat allocator remake')

        # Delete redundant tabs
        self.tab_display.removeTab(1)
        self.tab_display.setTabText(0, '原始')
        del self.tab_2
