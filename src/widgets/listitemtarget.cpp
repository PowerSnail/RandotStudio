#include "listitemtarget.h"
#include "uiListitemtarget.h"

ListItemTarget::ListItemTarget(QWidget* parent)
    : QWidget(parent), ui(new Ui::ListItemTarget) {
  ui->setupUi(this);
}

ListItemTarget::~ListItemTarget() { delete ui; }
