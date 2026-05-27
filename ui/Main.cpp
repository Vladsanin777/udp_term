#include <QApplication>
#include <QWidget>
#include <QWindow>
#include <QPushButton>
#include <QLineEdit>
#include <QTextEdit>
#include <QVBoxLayout>

class UDP_Window : public QWidget {
private:
    QVBoxLayout m_layout;
    QLineEdit m_ipSource;
    QLineEdit m_ipDestantion;
    QLineEdit m_portSource;
    QLineEdit m_portDestantion;
    QLineEdit m_macSource;
    QLineEdit m_macDestantion;
    QLineEdit m_interface;
    QTextEdit m_data;
    QLineEdit m_fileName;
    QPushButton m_send;
    bool m_initialized = false;

public:
    UDP_Window(QWidget * parent = nullptr) : QWidget{parent},
            m_layout{this}, m_send{"Send"} {
        setLayout(&m_layout);

        m_layout.addWidget(&m_ipSource);
        m_layout.addWidget(&m_ipDestantion);
        m_layout.addWidget(&m_portSource);
        m_layout.addWidget(&m_portDestantion);
        m_layout.addWidget(&m_macSource);
        m_layout.addWidget(&m_macDestantion);
        m_layout.addWidget(&m_interface);
        m_layout.addWidget(&m_data);
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
