#include "mainwindow.h"
#include <QtGui>
#include <QtWebKit>

MainWindow::MainWindow()
{
    progress = 0;
    QNetworkProxyFactory::setUseSystemConfiguration(true);
    view = new WebView(this);
    view->load(QUrl("http://www.sina.com.cn/"));
    connect(view, SIGNAL(loadFinished(bool)), SLOT(adjustLocation()));
    connect(view, SIGNAL(titleChanged(QString)), SLOT(adjustTitle()));
    connect(view, SIGNAL(loadProgress(int)), SLOT(setProgress(int)));
    connect(view, SIGNAL(loadFinished(bool)), SLOT(finishLoading(bool)));

    locationEdit = new QLineEdit(this);
    locationEdit->setSizePolicy(QSizePolicy::Expanding, locationEdit->sizePolicy().verticalPolicy());
    connect(locationEdit, SIGNAL(returnPressed()), SLOT(changeLocation()));

    QToolBar *toolBar = addToolBar(tr("Navigation"));
    toolBar->addAction(view->pageAction(QWebPage::Back));
    toolBar->addAction(view->pageAction(QWebPage::Forward));
    toolBar->addAction(view->pageAction(QWebPage::Reload));
    toolBar->addAction(view->pageAction(QWebPage::Stop));
    toolBar->addWidget(locationEdit);

    setCentralWidget(view);
    setUnifiedTitleAndToolBarOnMac(true);

    setContextMenuPolicy(Qt::NoContextMenu);
}

void MainWindow::adjustLocation()
{
    locationEdit->setText(view->url().toString());
}

void MainWindow::changeLocation()
{
    view->clearElementUrl();
    QUrl url = QUrl(locationEdit->text());
    view->load(url);
    view->setFocus();
}

void MainWindow::adjustTitle()
{
    if (progress <= 0 || progress >= 100)
        setWindowTitle(view->title());
    else
        setWindowTitle(QString("%1 (%2%)").arg(view->title()).arg(progress));
}

void MainWindow::setProgress(int p)
{
    progress = p;
    adjustTitle();
}

void MainWindow::finishLoading(bool)
{
    progress = 100;
    adjustTitle();
}

void MainWindow::highlightAllLinks()
{
    QString code = "$('a').each( function () { $(this).css('background-color', 'yellow') } )";
    view->page()->mainFrame()->evaluateJavaScript(code);
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    switch(event->key())
    {
    case Qt::Key_F6:
        view->outer();
        break;
    case Qt::Key_F7:
        view->inner();
        break;
    case Qt::Key_F8:
        view->genHtml();
        break;
    default:
        break;
    }
}
