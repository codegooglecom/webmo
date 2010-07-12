#include "webview.h"
#include <QtGui>
#include <qdebug.h>

WebView::WebView(QWidget *parent)
    : QWebView(parent)
{
    setContextMenuPolicy(Qt::NoContextMenu);
}

void WebView::genHtml()
{
    genElementUrl(&enclosingElement);
    QFile outF("output.htm");
    if(outF.open(QFile::WriteOnly|QFile::Truncate))
    {
        QTextStream out(&outF);
        QString str;
        str = "<!DOCTYPE html PUBLIC '-//W3C//DTD XHTML 1.0 Transitional//EN' 'http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd'>\n";
        str += "<html xmlns='http://www.w3.org/1999/xhtml'>\n";
        str += "<head>\n";
        str += "    <meta http-equiv='Content-Type' content='text/html; charset=gb2312' />\n";
        str += "    <title>Demo</title>\n";
        str += "    <script type='text/javascript' src='jquery.js'></script>\n";
        str += "</head>\n";
        str += "<body>\n";
        str += "<div id='result'></div>\n";
        str += "<div id='loadimage' style='display:none;'><img src='ajax-loader.gif' alt='loading...'/></div>\n";
        str += "<script type='text/javascript'>\n";
        str += "$(function getmessage() {\n";
        str += "$.ajax({\n";
        str += "    url:'" + url().toString() + "',\n";
        str += "    dataType:'html',\n";
        str += "    cache: false,\n";
        str += "    success:function(response){\n";
        str += "        $('#result').html($('";

        if(!idTag.isEmpty())
        {
            str += QString("%1#%2").arg(idTag).arg(idVal);
            for(int i=listAncestor.size()-1; i>=0; i--)
                str += QString(">%1:eq(%2)").arg(listAncestor[i]).arg(listIndex[i]);
        }
        else
        {
            if(listAncestor.size()<2)
            {
                QMessageBox::information(this, "Warning", "Too little ancestors!");
            }
            else
            {
                for(int i=listAncestor.size()-2; i>=0; i--)
                    str += QString("%1:eq(%2)").arg(listAncestor[i]).arg(listIndex[i]) + ((i==0)?"":">");
            }
        }
        //qDebug()<<listAncestor<<endl;
        //qDebug()<<listIndex<<endl;
        str += "', jQuery(response)).html());\n";
        str += "    },\n";
        str += "    beforeSend: function() {\n";
        str += "        document.getElementById('loadimage').style.display = 'block';\n";
        str += "    },\n";
        str += "    complete: function() {\n";
        str += "        $('#loadimage').hide();\n";
        str += "        setTimeout('getmessage()', 5000);\n";
        str += "    },\n";
        str += "    error: function() {\n";
        str += "        $('#result').html('error');\n";
        str += "    }\n";
        str += "})\n";
        str += "});\n";
        str += "</script>\n";
        str += "</body>\n";
        str += "</html>";
        str.replace("'","\"");
        out << str;
        outF.close();
    }
}

void WebView::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button()==Qt::RightButton)
    {
        enclosingElement = page()->mainFrame()->hitTestContent(event->pos()).enclosingBlockElement();
    }
    else
    {
        enclosingElement = QWebElement();
        QWebView::mouseReleaseEvent(event);
    }
    this->update();
}

void WebView::paintEvent(QPaintEvent *event)
{
    QWebView::paintEvent(event);
    QPainter painter(this);
    painter.setBrush(QBrush(Qt::blue, Qt::BDiagPattern));
    if(!enclosingElement.isNull())
        painter.drawRect(enclosingElement.geometry());
}

int getIndex(QWebElement *element)
{
    int index = 0;
    if(element->isNull() || element->parent().isNull())
        return -1;
    QWebElement *brother = &element->parent().firstChild();
    //qDebug()<<element->toOuterXml();
    while(!brother->isNull())
    {
        qDebug()<<brother->tagName()<<":"<<brother->classes()<<"      "<<element->tagName();
        if(brother->toOuterXml()==element->toOuterXml())
            return index;
        if(brother->tagName()==element->tagName())
            index++;
        brother=&brother->nextSibling();
    }
    return -1;
}

void WebView::genElementUrl(QWebElement *element)
{
    listAncestor.clear();
    listIndex.clear();
    idTag.clear();
    idVal.clear();
    while((element->tagName().toLower()=="table"||element->tagName().toLower()=="tbody") && !element->parent().isNull())
        element = &element->parent();
    for(int i=0;
        i<100 && element->tagName().toLower()!="body" && !element->tagName().isEmpty();
        i++, element=&element->parent())
    {
        if(element->hasAttribute("id"))
        {
            idTag = element->tagName().toLower();
            idVal = element->attribute("id", "");
            return;
        }
        listAncestor.append(element->tagName().toLower());
        listIndex.append(getIndex(element));
    }
}

void WebView::clearElementUrl()
{
    listAncestor.clear();
    listIndex.clear();
    idTag.clear();
    idVal.clear();
    enclosingElement = QWebElement();
    update();
}

void WebView::outer()
{
    if(enclosingElement.parent().isNull())
        QMessageBox::information(this, "Warning", "No more parent element!");
    else
        enclosingElement = enclosingElement.parent();
    update();
}

void WebView::inner()
{
    if(enclosingElement.firstChild().isNull())
        QMessageBox::information(this, "Warning", "No more child element!");
    else
        enclosingElement = enclosingElement.firstChild();
    update();
}
