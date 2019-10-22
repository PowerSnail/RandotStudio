#ifndef LABELEDLINEEDIT_H
#define LABELEDLINEEDIT_H

#include <QWidget>

namespace Ui {
class LabeledLineEdit;
}

class LabeledLineEdit : public QWidget {
  Q_OBJECT

 public:
  explicit LabeledLineEdit(QWidget* parent = nullptr);
  ~LabeledLineEdit();

 private:
  Ui::LabeledLineEdit* ui;
};

#endif  // LABELEDLINEEDIT_H
