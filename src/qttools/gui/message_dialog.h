/****************************************************************************
**
**  FougTools
**  Copyright FougSys (1 Mar. 2011)
**  contact@fougsys.fr
**
** This software is a computer program whose purpose is to provide utility
** tools for the C++ language and the Qt toolkit.
**
** This software is governed by the CeCILL-C license under French law and
** abiding by the rules of distribution of free software.  You can  use,
** modify and/ or redistribute the software under the terms of the CeCILL-C
** license as circulated by CEA, CNRS and INRIA at the following URL
** "http://www.cecill.info".
**
** As a counterpart to the access to the source code and  rights to copy,
** modify and redistribute granted by the license, users are provided only
** with a limited warranty  and the software's author,  the holder of the
** economic rights,  and the successive licensors  have only  limited
** liability.
**
** In this respect, the user's attention is drawn to the risks associated
** with loading,  using,  modifying and/or developing or reproducing the
** software by the user in light of its specific status of free software,
** that may mean  that it is complicated to manipulate,  and  that  also
** therefore means  that it is reserved for developers  and  experienced
** professionals having in-depth computer knowledge. Users are therefore
** encouraged to load and test the software's suitability as regards their
** requirements in conditions enabling the security of their systems and/or
** data to be ensured and,  more generally, to use and operate it in the
** same conditions as regards security.
**
** The fact that you are presently reading this means that you have had
** knowledge of the CeCILL-C license and that you accept its terms.
**
****************************************************************************/

#ifndef QTTOOLS_MESSAGE_DIALOG_H
#define QTTOOLS_MESSAGE_DIALOG_H

#include "qttools/gui/gui.h"
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
class QLabel;

namespace qttools {

class QTTOOLS_GUI_EXPORT MessageDialog : public QDialog
{
public:
  enum Icon
  {
    NoIcon,
    Information,
    Warning,
    Critical,
    Question
  };

  MessageDialog(Icon icon, const QString& title, const QString& text,
                QDialogButtonBox::StandardButtons buttons,
                QWidget* parent = 0);

  void setDetailsWidget(QWidget* widget);

private:
  QLabel* _textLabel;
  QWidget* _detailsWidget;
};

} // namespace qttools

#endif // QTTOOLS_MESSAGE_DIALOG_H
