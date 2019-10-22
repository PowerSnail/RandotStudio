#include "labeledlineedit.h"
#include "ui_labeledlineedit.h"

LabeledLineEdit::LabeledLineEdit(QWidget* parent)
    : QWidget(parent), ui(new Ui::LabeledLineEdit) {
  ui->setupUi(this);
}

LabeledLineEdit::~LabeledLineEdit() { delete ui; }
