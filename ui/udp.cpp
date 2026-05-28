#include <QApplication>
#include <QWidget>
#include <QWindow>
#include <QPushButton>
#include <QFormLayout>
#include <QLineEdit>
#include <QTextEdit>
#include <QCheckBox>
#include <QMouseEvent>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPoint>
#include <QLabel>
#include <QRect>
#include <QStyle>
#include <udp.h>
#include <stddef.h>


class UDP_Window : public QWidget {
private:
    QVBoxLayout * m_mainLayout{new QVBoxLayout{}};
    QHBoxLayout * m_titleLayout{new QHBoxLayout{}};
    QWidget * m_customTitleBar{new QWidget{}};
    QLabel * m_titleLabel{new QLabel{"UDP Transfer"}};
    QPushButton * m_minButton{new QPushButton{}};
    QPushButton * m_maxButton{new QPushButton{}};
    QPushButton * m_closeButton{new QPushButton{}};

    bool m_isDragging{false};
    QPoint * m_dragPosition{new QPoint{}};

    bool m_isResizing = false;
    int m_resizeEdge = 0;
    QPoint m_dragPos;
    QRect m_origGeometry;
    const int m_borderWidth = 8;

    QFormLayout * m_form{new QFormLayout{}};
    QLineEdit * m_ipSource{new QLineEdit{}};
    QLineEdit * m_ipDestantion{new QLineEdit{}};
    QLineEdit * m_portSource{new QLineEdit{}};
    QLineEdit * m_portDestantion{new QLineEdit{}};
    QLineEdit * m_macSource{new QLineEdit{}};
    QLineEdit * m_macDestantion{new QLineEdit{}};
    QLineEdit * m_interface{new QLineEdit{}};
    QCheckBox * m_fileCheckBox{new QCheckBox{}};
    QTextEdit * m_data{new QTextEdit{}};
    QLineEdit * m_fileName{new QLineEdit{}};
    QPushButton * m_send{new QPushButton{"Send"}};

    enum ResizeDirection {
        Left = 1,
        Right = 2,
        Top = 4,
        Bottom = 8
    };

public:
    UDP_Window(QWidget * parent = nullptr) : QWidget{parent} {
        customTitle();

        setLayout(m_mainLayout);

        m_mainLayout->setContentsMargins(0, 0, 0, 0);
        m_mainLayout->setSpacing(0);

        m_mainLayout->addLayout(m_form); 

        connect(m_fileCheckBox, &QCheckBox::toggled, this, &UDP_Window::onFileBoxToggle);
        connect(m_send, &QPushButton::clicked, this, &UDP_Window::clickSend);

        m_form->insertRow(0, "Ip source: ", m_ipSource);
        m_form->insertRow(1, "Ip destantion: ", m_ipDestantion);
        m_form->insertRow(2, "Port source: ", m_portSource);
        m_form->insertRow(3, "Port destantion: ", m_portDestantion);
        m_form->insertRow(4, "Mac source: ", m_macSource);
        m_form->insertRow(5, "Mac destantion: ", m_macDestantion);
        m_form->insertRow(6, "Interface: ", m_interface);
        m_form->insertRow(7, "File: ", m_fileCheckBox);
        m_fileCheckBox->setTristate(false);
        selectData();
        m_form->insertRow(9, m_send);

        m_send->setFixedHeight(40);

        m_form->setContentsMargins(10, 10, 10, 10);
    }

protected:
    virtual void customTitle(void) {
        setWindowFlags(Qt::Window | Qt::FramelessWindowHint); 

        m_customTitleBar->installEventFilter(this);
        m_titleLabel->installEventFilter(this);

        setMouseTracking(true);

        m_customTitleBar->setFixedHeight(40);

        m_titleLabel->setStyleSheet("color: white; font-weight: bold; margin-left: 10px;");

        m_minButton->setFixedSize(30, 30);
        m_minButton->setStyleSheet("background-color: #660066;");
        m_minButton->setIcon(style()->standardIcon(QStyle::SP_TitleBarMinButton));
        connect(m_minButton, &QPushButton::clicked, this, &QWidget::showMinimized);

        m_maxButton->setFixedSize(30, 30);
        m_maxButton->setStyleSheet("background-color: #880066;");
        m_maxButton->setIcon(style()->standardIcon(QStyle::SP_TitleBarMaxButton));
        connect(m_maxButton, &QPushButton::clicked, this, &QWidget::showMaximized);

        m_closeButton->setFixedSize(30, 30);
        m_closeButton->setStyleSheet("background-color: #880033;");
        m_closeButton->setIcon(style()->standardIcon(QStyle::SP_TitleBarCloseButton));
        connect(m_closeButton, &QPushButton::clicked, this, &QWidget::close);

        m_titleLayout->addWidget(m_titleLabel);
        m_titleLayout->addStretch();
        m_titleLayout->addWidget(m_minButton);
        m_titleLayout->addWidget(m_maxButton);
        m_titleLayout->addWidget(m_closeButton);
        m_titleLayout->setContentsMargins(0, 0, 5, 0);

        m_customTitleBar->setLayout(m_titleLayout);

        m_mainLayout->addWidget(m_customTitleBar);

        setObjectName("MainWindow");
        setStyleSheet(
                "QWidget#MainWindow {"
                "   background-color: #000033;"
                "   border: 1px solid #cccccc;"
                "   border-radius: 6px;"
                "}"
                "QPushButton {"
                "   border: 1px solid #cccccc;"
                "   background-color: #330066;"
                "   color: white;"
                "   border-radius: 6px;"
                "}"
        );
        m_customTitleBar->setObjectName("TitleBar");
        m_customTitleBar->setStyleSheet(
                "QWidget#TitleBar {"
                "   border: 1px solid #cccccc;"
                "   border-radius: 6px;"
                "   background-color: #330066;"
                "}"
        );
    }

    void mousePressEvent(QMouseEvent *event) override {
        if (event->button() == Qt::LeftButton) {
            if (event->position().y() <= 40) {
                if (windowHandle()) {
                    windowHandle()->startSystemMove();
                    event->accept();
                    return;
                }
            }
            if (m_resizeEdge > 0) {
                m_isResizing = true;
                m_dragPos = event->globalPosition().toPoint();
                m_origGeometry = geometry();
                event->accept();
            }
        } else {
            QWidget::mousePressEvent(event);
        }
    }

    void mouseMoveEvent(QMouseEvent *event) override {
        QPoint pos = event->position().toPoint();
        QPoint globalPos = event->globalPosition().toPoint();

        if (m_isResizing) {
            QRect currentGeom = this->geometry();
            
            int newX = currentGeom.x();
            int newY = currentGeom.y();
            int newW = currentGeom.width();
            int newH = currentGeom.height();

            QPoint mouseDelta = globalPos - m_dragPos;

            if (m_resizeEdge & Left) {
                int calculatedWidth = currentGeom.width() - mouseDelta.x();
                if (calculatedWidth >= minimumWidth()) {
                    newX = currentGeom.x() + mouseDelta.x();
                    newW = calculatedWidth;
                }
            }
            else if (m_resizeEdge & Right) {
                int calculatedWidth = currentGeom.width() + mouseDelta.x();
                if (calculatedWidth >= minimumWidth()) {
                    newW = calculatedWidth;
                }
            }

            if (m_resizeEdge & Top) {
                int calculatedHeight = currentGeom.height() - mouseDelta.y();
                if (calculatedHeight >= minimumHeight()) {
                    newY = currentGeom.y() + mouseDelta.y();
                    newH = calculatedHeight;
                }
            }
            else if (m_resizeEdge & Bottom) {
                int calculatedHeight = currentGeom.height() + mouseDelta.y();
                if (calculatedHeight >= minimumHeight()) {
                    newH = calculatedHeight;
                }
            }

            setGeometry(newX, newY, newW, newH);

            m_dragPos = globalPos;
            m_origGeometry = geometry();

            event->accept();
            return;
        }

        int edge = 0;
        if (pos.x() < m_borderWidth) edge |= Left;
        if (pos.x() > width() - m_borderWidth) edge |= Right;
        if (pos.y() < m_borderWidth) edge |= Top;
        if (pos.y() > height() - m_borderWidth) edge |= Bottom;

        m_resizeEdge = edge;

        if ((edge & Left && edge & Top) || (edge & Right && edge & Bottom)) {
            setCursor(Qt::SizeFDiagCursor);
        } else if ((edge & Right && edge & Top) || (edge & Left && edge & Bottom)) {
            setCursor(Qt::SizeBDiagCursor);
        } else if (edge & Left || edge & Right) {
            setCursor(Qt::SizeHorCursor);
        } else if (edge & Top || edge & Bottom) {
            setCursor(Qt::SizeVerCursor);
        } else {
            setCursor(Qt::ArrowCursor);
        }

        QWidget::mouseMoveEvent(event);
    }

    void mouseReleaseEvent(QMouseEvent *event) override {
        if (m_isResizing) {
            m_isResizing = false;
            m_resizeEdge = 0;
            setCursor(Qt::ArrowCursor);
            event->accept();
        } else {
            QWidget::mouseReleaseEvent(event);
        }
    }

private:
    void deleteText() {
        QFormLayout::TakeRowResult res = m_form->takeRow(8);
        
        if (res.labelItem) {
            if (QWidget *labelWidget = res.labelItem->widget()) {
                labelWidget->deleteLater();
            }
            delete res.labelItem;
        }
    }

    void selectFile(void) {
        m_data->hide();
        m_fileName->show();
        m_form->insertRow(8, "File name: ", m_fileName);
    }

    void selectData(void) {
        m_fileName->hide();
        m_data->show();
        m_form->insertRow(8, "Data: ", m_data);
    }

    void onFileBoxToggle(bool checked) {
        deleteText();
        checked ? selectFile() : selectData();
    }

    void clickSend() {
        ssize_t ret = 0;
        const char * ipSource = NULL;
        const char * ipDestantion = NULL;
        const char * portSource = NULL;
        const char * portDestantion = NULL;
        const char * macSource = NULL;
        const char * macDestantion = NULL;
        const char * interface = NULL;
        bool isFile = false;
        const char * data = NULL;
        const char * fileName = NULL;
        udp_pack_t pack = NULL;

        pack = init_udp_pack();

        if (pack == NULL) {
            goto getNotPack;
        }

        ipSource = qPrintable(m_ipSource->text());

        if (ipSource == NULL) {
            goto getNotIpSource;
        }

        ret = set_ip_address_source_udp_pack(pack, ipSource);

        if (ret != 0) {
            goto setNotIpSource;
        }

        ipDestantion = qPrintable(m_ipDestantion->text());

        if (ipDestantion == NULL) {
            goto getNotIpDestantion;
        }

        ret = set_ip_address_destantion_udp_pack(pack, ipDestantion);

        if (ret != 0) {
            goto setNotIpDestantion;
        }

        portSource = qPrintable(m_portSource->text());

        if (portSource == NULL) {
            goto getNotPortSource;
        }

        ret = set_port_source_udp_pack(pack, portSource);

        if (ret != 0) {
            goto setNotPortSource;
        }

        portDestantion = qPrintable(m_portDestantion->text());

        if (portDestantion == NULL) {
            goto getNotPortDestantion;
        }

        ret = set_port_destantion_udp_pack(pack, portDestantion);

        if (ret != 0) {
            goto setNotPortDestantion;
        }

        macSource = qPrintable(m_macSource->text());

        if (macSource == NULL) {
            goto getNotMacSource;
        }

        ret = set_mac_address_source_udp_pack(pack, macSource);

        if (ret != 0) {
            goto setNotMacSource;
        }

        macDestantion = qPrintable(m_macDestantion->text());

        if (macDestantion == NULL) {
            goto getNotMacDestantion;
        }

        ret = set_mac_address_destantion_udp_pack(pack, macDestantion);

        if (ret != 0) {
            goto setNotMacDestantion;
        }

        interface = qPrintable(m_interface->text());

        if (interface == NULL) {
            goto getNotInterface;
        }

        ret = set_interface_udp_pack(pack, interface);

        if (ret != 0) {
            goto setNotInterface;
        }

        isFile = m_fileCheckBox->isTristate();

        if (isFile == true) {

            fileName = qPrintable(m_fileName->text());

            if (fileName == NULL) {
                goto getNotFileName;
            }

            ret = set_file_data_udp_pack(pack, fileName);

            if (ret != 0) {
                goto setNotFileName;
            }

        } else {

            data = qPrintable(m_data->toPlainText());

            if (data == NULL) {
                goto getNotData;
            }

            ret = set_data_udp_pack(pack, data, strlen(data));

            if (ret != 0) {
                goto setNotData;
            }

        }

        ret = send_udp_pack(pack);

        if (ret != 0) {
            goto sendNotPack;
        }

        destroy_udp_pack(pack);
        
        return;

sendNotPack:
setNotData:
getNotData:
setNotFileName:
getNotFileName:
setNotInterface:
getNotInterface:
setNotMacDestantion:
getNotMacDestantion:
setNotMacSource:
getNotMacSource:
setNotPortDestantion:
getNotPortDestantion:
setNotPortSource:
getNotPortSource:
setNotIpDestantion:
getNotIpDestantion:
setNotIpSource:
getNotIpSource:
        destroy_udp_pack(pack);
getNotPack:
        m_customTitleBar->setStyleSheet("background-color: red;");
        return;
    }
};

int main(int argc, char ** argv) {
    QApplication app{argc, argv};
    UDP_Window window{};
    window.resize(350, 350);
    window.show();

    return app.exec();
}
