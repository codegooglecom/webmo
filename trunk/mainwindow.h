#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui>
#include "webview.h"

class QLineEdit;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();

protected slots:

    void adjustLocation();
    void changeLocation();
    void adjustTitle();
    void setProgress(int p);
    void finishLoading(bool);

    void highlightAllLinks();
    void keyPressEvent(QKeyEvent *);

private:
    WebView *view;
    QLineEdit *locationEdit;
    int progress;
};

#endif // MAINWINDOW_H
