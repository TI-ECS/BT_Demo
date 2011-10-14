#include "LoadingWidget.h"


LoadingWidget::LoadingWidget(QWidget *parent, Qt::WindowFlags f)
    :QDialog(parent, f)
{
    step = 0;

    QLinearGradient gradient(QPointF(50, -20), QPointF(80, 20));
    gradient.setColorAt(0.0, Qt::white);
    gradient.setColorAt(1.0, QColor(0xa6, 0xa6, 0xa6));

    background = QBrush(QColor(40,40,40));
    circleBrush = QBrush(gradient);
    circlePen = QPen(Qt::darkGray);
    circlePen.setWidth(1);
    textPen = QPen(Qt::white);
    textFont.setPixelSize(50);

    connect(&timer, SIGNAL(timeout()), this, SLOT(animate()));
}

LoadingWidget::~LoadingWidget()
{
}

void LoadingWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter;
    painter.begin(this);
    painter.setRenderHint(QPainter::Antialiasing);

    painter.fillRect(event->rect(), background);
    painter.translate(event->rect().center());
    painter.save();
    painter.setPen(circlePen);
    painter.translate(-138, 65);

    int n = 7;
    for (int i = 0; i < n; i++) {
        qreal radius;
        if (i == step) {
            painter.setBrush(circleBrush);
            radius = 16;
        } else {
            painter.setBrush(background);
            if(abs(i - step) == 1)
                radius = 14;
            else if (abs(i - step) == 2)
                radius = 12;
            else
                radius = 10;
        }

        painter.drawEllipse(QRectF(5, -radius,
                                   radius*2, radius*2));
        painter.translate(40, 0);
    }
    painter.restore();

    painter.setPen(textPen);
    painter.setFont(textFont);
    painter.drawText(QRect(-140, -60, 280, 120), Qt::AlignCenter, text);

    painter.end();
}

void LoadingWidget::animate()
{
    int aux = step;
    step = (step == 6) ? 0 : aux + 1;
    repaint();
}

void LoadingWidget::stop()
{
    timer.stop();
    hide();
}

void LoadingWidget::start(const QString &label)
{
    text = label;

    timer.start(190);
    showFullScreen();
}
