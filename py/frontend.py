from PyQt6.QtWidgets import QWidget, QFileDialog, QMessageBox, QTableWidgetItem, QHeaderView
from PyQt6.QtCore import Qt
from PyQt6.QtGui import QFont

from ui.ui_widget import Ui_Widget
import pandas as pd
import os

import grid_shuffler

class Frontend(QWidget, Ui_Widget):
    def __init__(self):
        super().__init__()
        self.setupUi(self)

        # Backend class
        self.__backend = grid_shuffler.Backend()

        # Window initialization
        self.setFixedSize(1000,750)
        self.setWindowTitle('seat allocator remake')

        # Delete redundant tabs
        self.tab_display.removeTab(1)
        self.tab_display.setTabText(0, '原始')
        del self.tab_2

        # Grid displayer config
        self.seat_viewer.horizontalHeader().setSectionResizeMode(QHeaderView.ResizeMode.Stretch)
        self.seat_viewer.verticalHeader().setSectionResizeMode(QHeaderView.ResizeMode.Stretch)

        # Button connectors
        self.csv_import.clicked.connect(self.import_data)

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
            self.display_data(data)

            self.csv_import_state.setText("已導入")
            self.csv_import.setText(os.path.basename(file_name))
            self.begin_shuffle.setEnabled(True)
        except RuntimeError:
            QMessageBox.warning(self, "錯誤", "讀取 CSV 檔案失敗。")

    #   import_data(self):
    #  =============================================================

    def display_data(self, d: int | list[list[str]]):
        try:
            data = self.__backend.set_data(d) if not isinstance(d, int) else self.__backend.get_grid(d)
        except Exception as e:
            print(f"backend failed with error: {e}")
            return

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
