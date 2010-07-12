#ifndef WEBVIEW_H
#define WEBVIEW_H

#include <QWebView>
#include <QtWebKit>

class WebView : public QWebView
{
public:
    WebView(QWidget* parent = 0);
    void mouseReleaseEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent*);
    void clearElementUrl();
    void genHtml();
    void outer();
    void inner();

private:
    void genElementUrl(QWebElement *webElement);

    QList<QString> listAncestor;
    QList<int> listIndex;
    QString idTag, idVal;
    QWebElement enclosingElement;
};

#endif // WEBVIEW_H
