#include <QWindow>
#include <QPushButton>
#include <QTextLine>
#include <QTextEdit>

class UDP_Window : public QWindow {
private:
    QVBoxLayout m_layout{this};
    QTextLine m_ipSource{&m_layout};
    QTextLine m_ipDestantion{&m_layout};
    QTextLine m_portSource{&m_layout};
    QTextLine m_portDestantion{&m_layout};
    QTextLine m_macSource{&m_layout};
    QTextLine m_macDestantion{&m_layout};
    QTextLine m_interface{&m_layout};
    QTextEdit m_data{&m_layout};
    QTextLine m_fileName{&m_layout};
    QPushButton m_send{&m_layout};

public:
    UDP_Window(QWindow * parent = nullptr) : QWindow{parent} {
        m_layout.addWidget(m_ipSource);
        m_layout.addWidget(m_ipDestantion);
        m_layout.addWidget(m_portSource);
        m_layout.addWidget(m_portDestantion);
        m_layout.addWidget(m_macSource);
        m_layout.addWidget(m_macDestantion);
        m_layout.addWidget(m_interface);
        m_layout.addWidget(m_data);
        m_layout.addWidget(m_fileName);
        m_layout.addWidget(m_send);

        setLayout(m_layout);
    }
};

int main() {
    UDP_Window window{};
    return 0;
}
