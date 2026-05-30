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
#include <QClipboard>
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

    QWidget * m_titleError{new QWidget{}};
    QHBoxLayout * m_errorLayout{new QHBoxLayout{}};
    QPushButton * m_closeError{new QPushButton{"⨉"}};
    QPushButton * m_copyError{new QPushButton{"⧉"}};
    QLabel * m_errorLabel{new QLabel{"Success"}};

    bool m_isDragging{false};
    QPoint * m_dragPosition{new QPoint{}};

    bool m_isResizing{false};
    Qt::Edges m_resizeEdge{};
    QPoint m_dragPos;
    QRect m_origGeometry;
    const int m_borderWidth{8};

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

public:
    UDP_Window(QWidget * parent = nullptr) : QWidget{parent} {
        customTitle();

        setLayout(m_mainLayout);

        resize(500, 500);

        m_mainLayout->setContentsMargins(0, 0, 0, 0);
        m_mainLayout->setSpacing(0);

        m_mainLayout->addLayout(m_form); 

        connect(m_fileCheckBox, &QCheckBox::toggled, this, &UDP_Window::onFileBoxToggle);

        m_form->insertRow(0, "Ip source: ", m_ipSource);
        m_form->insertRow(1, "Ip destantion: ", m_ipDestantion);
        m_form->insertRow(2, "Port source: ", m_portSource);
        m_form->insertRow(3, "Port destantion: ", m_portDestantion);
        m_form->insertRow(4, "Mac source: ", m_macSource);
        m_form->insertRow(5, "Mac destantion: ", m_macDestantion);
        m_form->insertRow(6, "Interface: ", m_interface);
        m_form->insertRow(7, "File: ", m_fileCheckBox);
        m_fileCheckBox->setTristate(false);

        selectFile();

        m_form->setContentsMargins(10, 10, 10, 10);
        m_form->setVerticalSpacing(5);
        m_form->setHorizontalSpacing(20);

        m_data->setTabChangesFocus(true);

        for (int i = 0; i < m_form->count(); ++i) {
            QLayoutItem* item = m_form->itemAt(i);
            if (QWidget* widget = item->widget()) {
                widget->setFixedHeight(30); 
                widget->setContentsMargins(0, 0, 0, 0);
            }
        }
        deleteText();
        selectData();
    }

protected:
    virtual void customTitle(void) {
        setWindowFlags(Qt::Window | Qt::FramelessWindowHint); 

        m_customTitleBar->installEventFilter(this);
        m_titleLabel->installEventFilter(this);

        setMouseTracking(true);

        m_customTitleBar->setFixedHeight(40);

        m_titleLabel->setStyleSheet("color: white; font-weight: bold;");

        createTitleError();

        m_send->setFixedSize(60, 30);
        m_send->setStyleSheet("background-color: #000033");
        connect(m_send, &QPushButton::clicked, this, &UDP_Window::clickSend);

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

        m_titleLayout->addWidget(m_send);
        m_titleLayout->addStretch();
        m_titleLayout->addWidget(m_titleLabel);
        m_titleLayout->addWidget(m_titleError);
        m_titleError->hide();
        m_titleLayout->addStretch();
        m_titleLayout->addWidget(m_minButton);
        m_titleLayout->addWidget(m_maxButton);
        m_titleLayout->addWidget(m_closeButton);
        m_titleLayout->setContentsMargins(5, 5, 5, 5);

        m_customTitleBar->setLayout(m_titleLayout);

        m_mainLayout->addWidget(m_customTitleBar);

        m_customTitleBar->setMouseTracking(true);
        m_titleLabel->setMouseTracking(true);

        setObjectName("MainWindow");
        setStyleSheet(
                "QWidget#MainWindow {"
                "   background-color: #000033;"
                "   border: 1px solid #cccccc;"
                "   border-radius: 6px;"
                "}"
                "QTextEdit, QPushButton,"
                "QLineEdit, QWidget#TitleBar {"
                "   border: 1px solid #cccccc;"
                "   background-color: #330066;"
                "   color: white;"
                "   border-radius: 6px;"
                "}"
                "QLabel {"
                "   color: white;"
                "}"
                "QCheckBox::indicator {"
                    "width: 18px;"
                    "height: 18px;"
                    "border: 2px solid #cccccc;"
                    "border-radius: 4px;"
                    "background-color: #330066;"
                "}"
                "QCheckBox::indicator:checked {"
                    "background-color: #880033;"
                "}"
        );
        m_customTitleBar->setObjectName("TitleBar");
        defaultTitle();
    }

    virtual void createTitleError(void) {
        m_titleError->setLayout(m_errorLayout);
        m_errorLayout->addWidget(m_closeError);
        m_errorLayout->addWidget(m_errorLabel);
        m_errorLayout->addWidget(m_copyError);

        m_titleError->setContentsMargins(0, 0, 0, 0);
        m_errorLayout->setContentsMargins(0, 0, 0, 0);

        m_closeError->setFixedSize(30, 30);
        m_closeError->setStyleSheet("background-color: #330066;");
        m_copyError->setFixedSize(30, 30);
        m_copyError->setStyleSheet("background-color: #500066;");

        connect(m_closeError, &QPushButton::clicked, this, &UDP_Window::defaultTitle);
        connect(m_copyError, &QPushButton::clicked, this, &UDP_Window::copyError);
    }

    bool eventFilter(QObject *watched, QEvent *event) override {
        if (m_isResizing || m_resizeEdge != 0) {
            return QWidget::eventFilter(watched, event);
        }

        if (watched == m_customTitleBar || watched == m_titleLabel) {
            if (event->type() == QEvent::MouseButtonPress) {
                QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
                
                if (mouseEvent->button() == Qt::LeftButton) {
                    if (windowHandle()) {
                        windowHandle()->startSystemMove();
                        mouseEvent->accept();
                        return true;
                    }
                }
            }
        }

        return QWidget::eventFilter(watched, event);
    }

    void mousePressEvent(QMouseEvent *event) override {
        QWindow * win = windowHandle();
        if (event->button() == Qt::LeftButton) {
            if (m_resizeEdge != 0) {
                if (win) {
                    win->startSystemResize(m_resizeEdge);
                    event->accept();
                    return;
                }
            }
            
            if (event->position().y() <= 40) {
                if (win) {
                    win->startSystemMove();
                    event->accept();
                    return;
                }
            }
        }

        QWidget::mousePressEvent(event);
    }

    void mouseDoubleClickEvent(QMouseEvent *event) {
        if (event->position().y() <= 40) {
            if (event->button() == Qt::LeftButton) {
                if (window()->isMaximized()) {
                    window()->showNormal();
                } else {
                    window()->showMaximized();
                }
                event->accept();
            } else {
                QWidget::mouseDoubleClickEvent(event);
            }
        }
    }
    void mouseMoveEvent(QMouseEvent *event) override {
        QPoint pos = event->position().toPoint();

        m_resizeEdge = Qt::Edges{};

        if (pos.x() < m_borderWidth) {
            m_resizeEdge |= Qt::LeftEdge;
        }

        if (pos.x() > width() - m_borderWidth) {
            m_resizeEdge |= Qt::RightEdge;
        }

        if (pos.y() < m_borderWidth) {
            m_resizeEdge |= Qt::TopEdge;
        }

        if (pos.y() > height() - m_borderWidth) {
            m_resizeEdge |= Qt::BottomEdge;
        }

        if ((m_resizeEdge & Qt::LeftEdge && m_resizeEdge & Qt::TopEdge) \
                || (m_resizeEdge & Qt::RightEdge && m_resizeEdge & Qt::BottomEdge)) {
            setCursor(Qt::SizeFDiagCursor);
        } else if ((m_resizeEdge & Qt::RightEdge && m_resizeEdge & Qt::TopEdge) \
                || (m_resizeEdge & Qt::LeftEdge && m_resizeEdge & Qt::BottomEdge)) {
            setCursor(Qt::SizeBDiagCursor);
        } else if (m_resizeEdge & Qt::LeftEdge || m_resizeEdge & Qt::RightEdge) {
            setCursor(Qt::SizeHorCursor);
        } else if (m_resizeEdge & Qt::TopEdge || m_resizeEdge & Qt::BottomEdge) {
            setCursor(Qt::SizeVerCursor);
        } else {
            setCursor(Qt::ArrowCursor);
        }

        QWidget::mouseMoveEvent(event);
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
        const char * errorMessage = "Success";
        udp_pack_t pack = NULL;

        pack = init_udp_pack();

        if (pack == NULL) {
            errorMessage = "Error: Get not Pack!";
            goto getNotPack;
        }

        ipSource = qPrintable(m_ipSource->text());

        if (ipSource == NULL) {
            errorMessage = "Error: Get not IP source!";
            goto getNotIpSource;
        }

        ret = set_ip_address_source_udp_pack(pack, ipSource);

        if (ret != 0) {
            errorMessage = "Error: Set not IP source!";
            goto setNotIpSource;
        }

        ipDestantion = qPrintable(m_ipDestantion->text());

        if (ipDestantion == NULL) {
            errorMessage = "Error: Get not IP destantion!";
            goto getNotIpDestantion;
        }

        ret = set_ip_address_destantion_udp_pack(pack, ipDestantion);

        if (ret != 0) {
            errorMessage = "Error: Set not IP destantion!";
            goto setNotIpDestantion;
        }

        portSource = qPrintable(m_portSource->text());

        if (portSource == NULL) {
            errorMessage = "Error: Get not port source!";
            goto getNotPortSource;
        }

        ret = set_port_source_udp_pack(pack, portSource);

        if (ret != 0) {
            errorMessage = "Error: Set not port source!";
            goto setNotPortSource;
        }

        portDestantion = qPrintable(m_portDestantion->text());

        if (portDestantion == NULL) {
            errorMessage = "Error: Get not port destantion!";
            goto getNotPortDestantion;
        }

        ret = set_port_destantion_udp_pack(pack, portDestantion);

        if (ret != 0) {
            errorMessage = "Error: Set not port destantion!";
            goto setNotPortDestantion;
        }

        macSource = qPrintable(m_macSource->text());

        if (macSource == NULL) {
            errorMessage = "Error: Get not MAC source!";
            goto getNotMacSource;
        }

        ret = set_mac_address_source_udp_pack(pack, macSource);

        if (ret != 0) {
            errorMessage = "Error: Set not MAC source!";
            goto setNotMacSource;
        }

        macDestantion = qPrintable(m_macDestantion->text());

        if (macDestantion == NULL) {
            errorMessage = "Error: Get not MAC destantion!";
            goto getNotMacDestantion;
        }

        ret = set_mac_address_destantion_udp_pack(pack, macDestantion);

        if (ret != 0) {
            errorMessage = "Error: Set not MAC destantion!";
            goto setNotMacDestantion;
        }

        interface = qPrintable(m_interface->text());

        if (interface == NULL) {
            errorMessage = "Error: Get not interface!";
            goto getNotInterface;
        }

        ret = set_interface_udp_pack(pack, interface);

        if (ret != 0) {
            errorMessage = "Error: Set not interface!";
            goto setNotInterface;
        }

        isFile = m_fileCheckBox->isTristate();

        if (isFile == true) {

            fileName = qPrintable(m_fileName->text());

            if (fileName == NULL) {
                errorMessage = "Error: Get not file name!";
                goto getNotFileName;
            }

            ret = set_file_data_udp_pack(pack, fileName);

            if (ret != 0) {
                errorMessage = "Error: Set not file name!";
                goto setNotFileName;
            }

        } else {

            data = qPrintable(m_data->toPlainText());

            if (data == NULL) {
                errorMessage = "Error: Get not data!";
                goto getNotData;
            }

            ret = set_data_udp_pack(pack, data, strlen(data));

            if (ret != 0) {
                errorMessage = "Error: Set not data!";
                goto setNotData;
            }

        }

        ret = send_udp_pack(pack);

        if (ret != 0) {
            errorMessage = "Error: Send not pack!";
            goto sendNotPack;
        }

        destroy_udp_pack(pack);

        successTitle();
        
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
        errorTitle(errorMessage);
        return;
    }

    void defaultTitle(void) {
        m_titleError->hide();
        m_titleLabel->show();
        m_customTitleBar->setStyleSheet("background-color: #330066;");
    }

    void errorTitle(const char * message) {
        m_titleLabel->hide();
        m_titleError->show();
        m_errorLabel->setText(message);
        m_customTitleBar->setStyleSheet("background-color: red;");
    }

    void successTitle(void) {
        m_titleLabel->hide();
        m_titleError->show();
        m_errorLabel->setText("Success: send!");
        m_customTitleBar->setStyleSheet("background-color: green;");
    }

    void copyError(void) {
        QClipboard* clipboard = QApplication::clipboard();
        const char * errorMessage = qPrintable(m_errorLabel->text());
        clipboard->setText(errorMessage);
    }
};

int main(int argc, char ** argv) {
    QApplication app{argc, argv};
    UDP_Window window{};
    window.show();

    return app.exec();
}
