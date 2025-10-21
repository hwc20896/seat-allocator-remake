from PyQt6.QtWidgets import QWidget, QFileDialog, QMessageBox, QTableWidgetItem
from PyQt6.QtCore import Qt
from PyQt6.QtGui import QFont

from ui.ui_widget import Ui_Widget
import pandas as pd

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

    #   __init__(self)  ^^^^^
    #  =============================================================

    def import_data(self):
        # Private single-use function no. 1
        def read_csv(file_path):
            try:
                df = pd.read_csv(file_path, header=None, dtype=str, engine='c')
                return df.fillna('').values.tolist()
            except Exception:
                raise RuntimeError('Error when reading csv')

        file_name, _ = QFileDialog.getOpenFileName(self, '導入csv', '', 'csv檔 (*.csv)')
        if not file_name:
            return

        print(f'Selected file: {file_name}')
        try:
            data = read_csv(file_name)
        except RuntimeError:
            QMessageBox.warning(self, "錯誤", "讀取 CSV 檔案失敗。")

    #   import_data(self):
    #  =============================================================

    def display_data(self, d: int | list[list[int]]):
        data = d if not isinstance(d, int) else []

        if not data: return

        row = len(data)
        col = len(data[0]) if row > 0 else 0

        self.seat_viewer.setRowCount(row)
        self.seat_viewer.setColumnCount(col)

        for i in range(row):
            for j in range(col):
                item = QTableWidgetItem(data[i][j])
                item.setTextAlignment(Qt.AlignmentFlag.AlignCenter)
                item.setFont(QFont("Microsoft JhengHei", 16))
                self.seat_viewer.setItem(i, j, item)
