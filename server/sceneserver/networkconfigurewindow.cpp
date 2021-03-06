// generated by Fast Light User Interface Designer (fluid) version 1.0303

#include "networkconfigurewindow.hpp"

NetworkConfigureWindow::NetworkConfigureWindow() {
  { m_Window = new Fl_Double_Window(510, 283, "Network Configure");
    m_Window->labelfont(4);
    m_Window->user_data((void*)(this));
    { m_GateServerIP = new Fl_Input(172, 16, 150, 24, "GateServer IP: ");
      m_GateServerIP->labelfont(4);
      m_GateServerIP->textfont(4);
    } // Fl_Input* m_GateServerIP
    { m_GateServerPort = new Fl_Value_Input(172, 53, 60, 24, "GateServer Port: ");
      m_GateServerPort->labelfont(4);
      m_GateServerPort->minimum(1024);
      m_GateServerPort->maximum(10000);
      m_GateServerPort->value(3500);
      m_GateServerPort->textfont(4);
    } // Fl_Value_Input* m_GateServerPort
    { m_DatabaseIP = new Fl_Input(172, 90, 150, 24, "Database IP: ");
      m_DatabaseIP->labelfont(4);
      m_DatabaseIP->textfont(4);
    } // Fl_Input* m_DatabaseIP
    { m_DatabasePort = new Fl_Value_Input(172, 127, 60, 24, "Database Port: ");
      m_DatabasePort->labelfont(4);
      m_DatabasePort->minimum(1024);
      m_DatabasePort->maximum(10000);
      m_DatabasePort->value(3306);
      m_DatabasePort->textfont(4);
    } // Fl_Value_Input* m_DatabasePort
    { m_UserName = new Fl_Input(172, 203, 150, 24, "User Name: ");
      m_UserName->labelfont(4);
      m_UserName->textfont(4);
    } // Fl_Input* m_UserName
    { m_Password = new Fl_Input(172, 240, 150, 24, "Password: ");
      m_Password->type(5);
      m_Password->labelfont(4);
      m_Password->textfont(4);
    } // Fl_Input* m_Password
    { m_DatabaseName = new Fl_Input(172, 165, 150, 24, "Database Name: ");
      m_DatabaseName->labelfont(4);
      m_DatabaseName->textfont(4);
    } // Fl_Input* m_DatabaseName
    m_Window->end();
  } // Fl_Double_Window* m_Window
  m_DatabaseIP->value("127.0.0.1");
  m_GateServerIP->value("127.0.0.1");
  m_UserName->value("root");
  m_Password->value("hah86918");
  m_DatabaseName->value("mir2x");
}

void NetworkConfigureWindow::ShowAll() {
  {
      m_Window->show();
  }
}

const char * NetworkConfigureWindow::GateServerIP() {
  {
      return (m_GateServerIP->value());
  }
}

int NetworkConfigureWindow::GateServerPort() {
  {
      // printf("%d\n", std::lround(m_VIPort1->value()));
  
      return std::lround(m_GateServerPort->value());
  }
}

const char * NetworkConfigureWindow::DatabaseIP() {
  {
      return m_DatabaseIP->value();
  }
}

int NetworkConfigureWindow::DatabasePort() {
  {
      return std::lround(m_DatabasePort->value());
  }
}

const char * NetworkConfigureWindow::DatabaseName() {
  {
      return m_DatabaseName->value();
  }
}

const char * NetworkConfigureWindow::UserName() {
  {
      return m_UserName->value();
  }
}

const char * NetworkConfigureWindow::Password() {
  {
      return m_Password->value();
  }
}
