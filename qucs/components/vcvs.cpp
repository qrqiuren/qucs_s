/***************************************************************************
                          vcvs.cpp  -  description
                             -------------------
    begin                : Sat Aug 23 2003
    copyright            : (C) 2003 by Michael Margraf
    email                : michael.margraf@alumni.tu-berlin.de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "vcvs.h"
#include "node.h"
#include "misc.h"
#include "extsimkernels/spicecompat.h"
#include "extsimkernels/verilogawriter.h"


VCVS::VCVS()
{
  Description = QObject::tr("voltage controlled voltage source");

  Arcs.append(new qucs::Arc(0,-11, 22, 22,  0, 16*360,QPen(Qt::darkBlue,2)));

  Lines.append(new qucs::Line(-30,-30,-12,-30,QPen(Qt::darkBlue,2)));
  Lines.append(new qucs::Line(-30, 30,-12, 30,QPen(Qt::darkBlue,2)));
  Lines.append(new qucs::Line( 11,-30, 30,-30,QPen(Qt::darkBlue,2)));
  Lines.append(new qucs::Line( 11, 30, 30, 30,QPen(Qt::darkBlue,2)));

  Lines.append(new qucs::Line(-12,-30,-12,-23,QPen(Qt::darkBlue,2)));
  Lines.append(new qucs::Line(-12, 30,-12, 23,QPen(Qt::darkBlue,2)));
  Lines.append(new qucs::Line( 11,-30, 11,-11,QPen(Qt::darkBlue,2)));
  Lines.append(new qucs::Line( 11, 30, 11, 11,QPen(Qt::darkBlue,2)));

  Lines.append(new qucs::Line(-12,-18,-12, 18,QPen(Qt::darkBlue,1)));
  Lines.append(new qucs::Line(-12, 18,-17,  9,QPen(Qt::darkBlue,1)));
  Lines.append(new qucs::Line(-12, 18, -7,  9,QPen(Qt::darkBlue,1)));

  Lines.append(new qucs::Line( 19,-21, 19,-15,QPen(Qt::red,1)));
  Lines.append(new qucs::Line( 16,-18, 22,-18,QPen(Qt::red,1)));
  Lines.append(new qucs::Line( 16, 18, 22, 18,QPen(Qt::black,1)));

  Lines.append(new qucs::Line(-25,-27, 25,-27,QPen(Qt::darkGray,1)));
  Lines.append(new qucs::Line( 25,-27, 25, 27,QPen(Qt::darkGray,1)));
  Lines.append(new qucs::Line( 25, 27,-25, 27,QPen(Qt::darkGray,1)));
  Lines.append(new qucs::Line(-25, 27,-25,-27,QPen(Qt::darkGray,1)));

  Ports.append(new Port(-30,-30));
  Ports.append(new Port( 30,-30));
  Ports.append(new Port( 30, 30));
  Ports.append(new Port(-30, 30));

  x1 = -30; y1 = -30;
  x2 =  30; y2 =  30;

  tx = x1+4;
  ty = y2+4;
  Model = "VCVS";
  Name  = "SRC";
  SpiceModel = "E";

  Props.append(new Property("G", "1", true,
		QObject::tr("forward transfer factor")));
  Props.append(new Property("T", "0", false, QObject::tr("delay time")));
}

VCVS::~VCVS()
{
}

Component* VCVS::newOne()
{
  return new VCVS();
}

Element* VCVS::info(QString& Name, char* &BitmapFile, bool getNewOne)
{
  Name = QObject::tr("Voltage Controlled Voltage Source");
  BitmapFile = (char *) "vcvs";

  if(getNewOne)  return new VCVS();
  return 0;
}

QString VCVS::va_code()
{
    QString Gain = vacompat::normalize_value(Props.at(0)->Value);
	QString P1 = Ports.at(0)->Connection->Name;
    QString P4 = Ports.at(1)->Connection->Name;
    QString P3 = Ports.at(2)->Connection->Name;
    QString P2 = Ports.at(3)->Connection->Name;
    QString s = "";
    
    QString Vpm = vacompat::normalize_voltage(P1,P2);
    QString Ipm = vacompat::normalize_current(P1,P2,true);  
    s += QString(" %1  <+  %2 * 1e-9;\n").arg(Ipm).arg(Vpm);
    QString Vpm2 = vacompat::normalize_voltage(P3,P4);
    QString Ipm2 = vacompat::normalize_current(P3,P4,true); 
    s += QString("%1  <+  -(%2 * 1e3);\n").arg(Ipm2).arg(Vpm2);
    s += QString("%1  <+  -(%2 * 1e3*  %3) ;\n").arg(Ipm2).arg(Vpm).arg(Gain);
    
    return s;
 }
QString VCVS::spice_netlist(bool)
{
   QString s = spicecompat::check_refdes(Name,SpiceModel);
    QList<int> seq; // nodes sequence
    seq<<1<<2<<0<<3;
    // output all node names
    for (int i : seq) {
        QString nam = Ports.at(i)->Connection->Name;
        if (nam=="gnd") nam = "0";
        s += " "+ nam;   // node names
    }

    s += " " + Props.at(0)->Value + "\n"; // Ignore delay time. It is spice-incompatibele

    return s;
}
