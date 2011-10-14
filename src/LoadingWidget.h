#ifndef _LOADINGWIDGET_H_
#define _LOADINGWIDGET_H_

#include <QtGui>

class LoadingWidget : public QDialog
{
Q_OBJECT

public:
    LoadingWidget(QWidget *parent = 0, Qt::WindowFlags f = 0);
    virtual ~LoadingWidget();

private slots:
    void animate();

public slots:
    void start(const QString &label);
    void stop();

protected:
    virtual void paintEvent(QPaintEvent *event);

private:
    QBrush background;
    QBrush circleBrush;
    QFont textFont;
    QPen circlePen;
    QPen textPen;
    QString text;
    QTimer timer;
    int step;
};

#endif /* _LOADINGWIDGET_H_ */
