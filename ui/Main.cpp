#include <QApplication>
#include <QLabel>
#include <QWidget>
#include <QWindow>
#include <QPushButton>
#include <QLineEdit>
#include <QTextEdit>
#include <QVBoxLayout>

class UDP_Window : public QWidget {
private:
    QVBoxLayout m_layout;
    QLabel m_ipSourceLabel{"Ip sourse"};
    QLineEdit m_ipSource;
    QLabel m_ipDestantionLabel{"Ip destantion"};
    QLineEdit m_ipDestantion;
    QLabel m_portSourceLabel{"Port sourse"};
    QLineEdit m_portSource;
    QLabel m_portDestantionLabel{"Port destantion"};
    QLineEdit m_portDestantion;
    QLabel m_macSourceLabel{"Mac sourse"};
    QLineEdit m_macSource;
    QLabel m_macDestantionLabel{"Mac destantion"};
    QLineEdit m_macDestantion;
    QLabel m_interfaceLabel{"Interface"};
    QLineEdit m_interface;
    QLabel m_dataLabel{"Data"};
    QTextEdit m_data;
    QLabel m_fileNameLabel{"File name"};
    QLineEdit m_fileName;
    QPushButton m_send{"Send"};

public:
    UDP_Window(QWidget * parent = nullptr) : QWidget{parent},
            m_layout{this} {
        setLayout(&m_layout);

        m_layout.addWidget(&m_ipSourceLabel);
        m_layout.addWidget(&m_ipSource);
        m_layout.addWidget(&m_ipDestantionLabel);
        m_layout.addWidget(&m_ipDestantion);
        m_layout.addWidget(&m_portSourceLabel);
        m_layout.addWidget(&m_portSource);
        m_layout.addWidget(&m_portDestantionLabel);
        m_layout.addWidget(&m_portDestantion);
        m_layout.addWidget(&m_macSourceLabel);
        m_layout.addWidget(&m_macSource);
        m_layout.addWidget(&m_macDestantionLabel);
        m_layout.addWidget(&m_macDestantion);
        m_layout.addWidget(&m_interfaceLabel);
        m_layout.addWidget(&m_interface);
        m_layout.addWidget(&m_dataLabel);
        m_layout.addWidget(&m_data);
        m_layout.addWidget(&m_fileNameLabel);
        m_layout.addWidget(&m_fileName);
        m_layout.addWidget(&m_send);
    }
};

int main(int argc, char ** argv) {
    QApplication app{argc, argv};
    UDP_Window window{};
    window.resize(350, 350);
    window.show();
    return app.exec();
}
