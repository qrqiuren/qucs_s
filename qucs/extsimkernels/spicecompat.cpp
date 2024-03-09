#include "spicecompat.h"
#include "main.h"
#include "misc.h"

#include <QDebug>

/*!
 * \brief spicecompat::check_refdes If starting letters of the component name
 *        match spice model (i.e. R1, C1, L1)returns Name. Otherwise function
 *        returns an unique Spice Refdes (Name+SpiceModel)
 * \param Name
 * \param SpiceModel
 * \return
 */
QString spicecompat::check_refdes(QString &Name,QString &SpiceModel)
{
    if (Name.startsWith(SpiceModel,Qt::CaseInsensitive)) {
        return Name;
    } else {
        return (SpiceModel + Name);
    }
}

/*!
 * \brief spicecompat::normalize_value Remove units from component values and
 *        replace Spice-incompatible factors (i.e M, Meg). Wrap value in braces
 *        if it contains variables.
 * \param[in] Value Qucs-style component value
 * \return Spice-style component value
 */
QString spicecompat::normalize_value(QString Value)
{
    const QRegularExpression r_pattern("^[0-9]+.*Ohm$");
    const QRegularExpression p_pattern("^[+-]*[0-9]+.*dBm$");
    const QRegularExpression c_pattern("^[0-9]+.*F$");
    const QRegularExpression l_pattern("^[0-9]+.*H$");
    const QRegularExpression v_pattern("^[0-9]+.*V$");
    const QRegularExpression i_pattern("^[0-9]+.*A$");
    const QRegularExpression hz_pattern("^[0-9]+.*Hz$");
    const QRegularExpression s_pattern("^[0-9]+.*S$");
    const QRegularExpression sec_pattern("^[0-9]+.*s$");
    const QRegularExpression var_pattern("^[A-Za-z].*$");

    QString s = Value.remove(' ');
    if (s.startsWith('\'')&&s.endsWith('\'')) return Value; // Expression detected

    if (r_pattern.match(s).hasMatch()) { // Component value
        s.remove("Ohm");
        s.replace("M","Meg");
    } else if (c_pattern.match(s).hasMatch()) {
        s.remove("F");
        s.replace("M","Meg");
    } else if (l_pattern.match(s).hasMatch()) {
        s.remove("H");
        s.replace("M","Meg");
    } else if (v_pattern.match(s).hasMatch()) {
        s.remove("V");
        s.replace("M","Meg");
    } else if (i_pattern.match(s).hasMatch()) {
        s.remove("A");
        s.replace("M","Meg");
    } else if (hz_pattern.match(s).hasMatch()) {
        s.remove("Hz");
        s.replace("M","Meg");
    } else if (s_pattern.match(s).hasMatch()) {
        s.remove("S");
        s.replace("M","Meg");
    } else if (sec_pattern.match(s).hasMatch()) {
        s.remove("s");
        s.replace("M","Meg");
    } else if (p_pattern.match(s).hasMatch()) {
        s.remove("dBm");
    } else if (var_pattern.match(s).hasMatch()) {
        s = "{" + s + "}";
    }

    return s.toUpper();
}

/*!
 * \brief spicecompat::convert_functions Convert Qucs mathematical function name
 *        to Spice mathematical function name.
 * \param tok[in] Qucs function name
 * \param isXyce[in] True if Xyce is used, False by default.
 * \return
 */
QString spicecompat::convert_function(QString tok, bool isXyce)
{
    const QStringList conv_list_ngspice = {"q","1.6021765e-19",
                                           "kB","1.38065e-23",
                                           "pi","3.1415926539",
                                           "step","stp",
                                           "sign","sgn",
                                           "log10","log"};
    // Put here functions need to be converted
//    conv_list_ngspice<<"q"<<"1.6021765e-19"
//            <<"kB"<<"1.38065e-23"
//            <<"pi"<<"3.1415926539"
//            <<"step"<<"stp"
//            <<"sign"<<"sgn"
//            <<"log10"<<"log";
    QStringList conv_list_xyce = conv_list_ngspice;

    QStringList conv_list;
    if (isXyce) conv_list = conv_list_xyce;
    else conv_list = conv_list_ngspice;

    for(int i=0;i<conv_list.count();i+=2) {
        if (conv_list.at(i)==tok)
            return conv_list.at(i+1);
    }
    return tok;
}

/*!
 * \brief spicecompat::convert_functions Convert Qucs mathematical function name
 *        to Spice mathematical function name
 * \param tokens[in/out] String list in which to put tokens
 * \param isXyce[in] True if Xyce is used, False by default.
 */
void spicecompat::convert_functions(QStringList &tokens, bool isXyce)
{
    for(QStringList::iterator it = tokens.begin();it != tokens.end(); it++) {
        *it = spicecompat::convert_function(*it,isXyce);
    }
}

/*!
 * \brief spicecompat::splitEqn Split Equation into tokens. Token delimenters are mathamatical
 *        operation signs:  = ^ + - * /  and parentheses ()
 * \param[in] eqn Equation to split
 * \param[out] tokens String list in which to put tokens.
 */
void spicecompat::splitEqn(QString &eqn, QStringList &tokens)
{
    tokens.clear();
    QString tok = "";
    const QString delim = "=()*/+-^<>:?&>=%!|";
    for (QString::iterator it=eqn.begin();it!=eqn.end();it++) {
        if (it->isSpace()) continue;
        if (delim.contains(*it)) {
            if (!tok.isEmpty()) tokens.append(tok);
            tokens.append(*it);
            tok.clear();
            continue;
        }
        tok += *it;
    }
    if (!tok.isEmpty()) tokens.append(tok);
    const QRegularExpression fpn("[0-9]+\\.[0-9]+[eE]"); // first part of float number
    const QRegularExpression dn("[0-9]+[eE]");
    const QRegularExpression intn("[0-9]+");
    // Reassemble floating point numbers such as [+-]1.2e[+-]02 , etc.
    for(auto t = tokens.begin();t != tokens.end();t++) {

        qDebug()<<*t;
        if (dn.match(*t).hasMatch()||fpn.match(*t).hasMatch()) {
            auto t1 = t;
            t1++;
            if (t1!=tokens.end()) {
                if ((*t1=="+")||(*t1=="-")) {
                    (*t) += (*t1);
                    *t1 = "";
                }
            } else break;
            t1++;
            if (t1!=tokens.end()) {

                if (intn.match(*t1).hasMatch()) {
                    (*t) += (*t1);
                    *t1 = "";
                }
            } else break;
        }
    }
    tokens.removeAll("");
}


/*!
 * \brief spicecompat::containNodes Determine are there in equaton node voltages
 *        and/or current porbes.
 * \param tokens List of tokens. Should be obtained with splitEqn().
 * \param vars List of vars that are used in ngnutmeg script.
 * \return Return true if equation contain node voltages and/or current probe variables
 */
bool spicecompat::containNodes(QStringList &tokens, QStringList &vars)
{
    const QRegularExpression var_pattern("^[\\w]+\\.([IV]t|[iv]|vn|Vb|[IV])$");
    const QRegularExpression disto_var("^[Dd][Ii][Ss][Tt][Oo][0-9]\\.[Vv]$");
    QStringList system_vars;
    system_vars.clear();
    system_vars<<"frequency"<<"acfrequency"<<"time"<<"hbfrequncy";
    for (const QString& tok : tokens) {
        if (var_pattern.match(tok).hasMatch()) return true;
        if (disto_var.match(tok).hasMatch()) return true;
        if (system_vars.contains(tok)) return true;
        if (tok.endsWith("#branch")) return true;
        if (vars.contains(tok)) return true;
    }
    for (QStringList::iterator it=tokens.begin();it!=tokens.end();it++) {
        if ((*it).toUpper()=="V") { // voltages  in spice notation
            it++;
            if ((*it)=="(") return true;
        }
    }
    return false;
}

/*!
 * \brief spicecompat::convertNodeNames convert node names form Qucs-notation to
 *        Spice-notation (i.e. Node.Vt --> V(Node) ) and determine used simulation.
 *        This method modifies the input list of tokens.
 * \param[in/out] tokens
 * \param[out] sim Used simulation.
 *             "ac" --- AC simulation;
 *             "disto" --- DISTO simulation;
 *             "dc" --- DC simulation;
 *             "tran" --- Transient simulation;
 *             "all" --- All simulations;
 */
void spicecompat::convertNodeNames(QStringList &tokens, QString &sim)
{
    const QRegularExpression var_pattern("^[\\w]+\\.([IV]t|[iv]|vn|Vb|[IV])$");
    const QRegularExpression disto_var("^[Dd][Ii][Ss][Tt][Oo][0-9]\\.[Vv]$");
    for (QStringList::iterator it=tokens.begin();it!=tokens.end();it++) {
        if ((*it).endsWith("#branch")) sim="all";
        if ((*it).toUpper()=="V") {
            it++;
            if ((*it)=="(") sim="all";
        }
        if (disto_var.match(*it).hasMatch()) sim = "disto";
        if (var_pattern.match(*it).hasMatch())  {
            if (!disto_var.match(*it).hasMatch()) {
                if ((it->endsWith(".v"))||(it->endsWith(".i"))) sim = "ac";
                if ((it->endsWith(".Vt"))||(it->endsWith(".It"))) sim = "tran";
                if ((it->endsWith(".V"))||(it->endsWith(".I"))) sim = "dc";
                QString suffix = it->section('.',1,1);
                auto idx = it->indexOf('.');
                auto cnt = it->size();
                it->chop(cnt-idx);
                if (suffix.toUpper().startsWith("I"))
                    *it = QString("V%1#branch").arg(*it);
                else *it = QString("V(%2)").arg(*it);
            }
        } else if ((*it=="frequency")||(*it=="acfrequency")) {
            sim = "ac";
        } else if (*it=="time") {
            sim = "tran";
        }
    }
}

QString spicecompat::normalize_node_name(QString nod) {
    return (nod == "gnd") ? QString("0") : nod;
}

QString spicecompat::convert_relative_filename(QString filename)
{
    QFileInfo inf(filename);
    if (inf.exists()) return filename;

    QString s = QucsSettings.QucsWorkDir.absolutePath() + QDir::separator() + filename;
    inf.setFile(s);
    return inf.exists() ? s : filename;
}

int spicecompat::getPins(const QString &file, const QString &compname, QStringList &pin_names)
{
    int r = 0;
    QString content;
    QString LibName = spicecompat::convert_relative_filename(file);
    QFile f(LibName);
    if (f.open(QIODevice::ReadOnly)) {
        QTextStream ts(&f);
        content = ts.readAll();
        f.close();
    } else return 0;

    const QRegularExpression subckt_header("^\\s*\\.(S|s)(U|u)(B|b)(C|c)(K|k)(T|t)\\s.*");
    const QRegularExpression sep("\\s");

    QTextStream stream(&content,QIODevice::ReadOnly);
    while (!stream.atEnd()) {
        QString lin = stream.readLine();

        if (subckt_header.match(lin).hasMatch()) {

            QStringList lst2 = lin.split(sep,qucs::SkipEmptyParts);
            QString name = lin.section(sep,1,1,QString::SectionSkipEmpty).toLower();
            QString refname = compname.toLower();
            if (name != refname) continue;
            lst2.removeFirst();
            lst2.removeFirst();
            for (const auto &s1: lst2) {
                QString pp = s1;
                if (!s1.contains('=') &&
                   (pp.toLower() != "params:")) {
                    pin_names.append(s1);
                }
            }
            r = pin_names.count();
            break;
        }
    }

    return r;
}

/*!
 * \brief spicecompat::getSubcktName
 * \param subfilename file containing subcircuit definition
 * \return .SUBCKT entry name
 */
QString spicecompat::getSubcktName(const QString& subfilename)
{
    QString s = "";

    QFile sub_file(subfilename);
    if (sub_file.open(QIODevice::ReadOnly)) {
        const QRegularExpression subckt_header("^\\s*\\.(S|s)(U|u)(B|b)(C|c)(K|k)(T|t)\\s.*");
        const QRegularExpression sep("\\s");
        QStringList lst = QString(sub_file.readAll()).split("\n");
        for (const QString& str : lst) {            
            if (subckt_header.match(str).hasMatch()) {
                s = str.section(sep,1,1,QString::SectionSkipEmpty);
                break;
            }
        }
        sub_file.close();
    }
    return s;
}

/*!
 * \brief spicecompat::convert_sweep_type Convert sweep mode (i.e. linear, logarithmic, etc.)
 *        from Qucs notation to SPICE notation
 * \param sweep Sweep designation in Qucs notation
 * \return Sweep designation in SPICE notation
 */
QString spicecompat::convert_sweep_type(const QString& sweep)
{
    if (sweep=="lin") return QString("lin");
    if (sweep=="log") return QString("dec");
    return QString("");
}

/*!
 * \brief spicecompat::check_nodename Check if node name is Nutmeg keyword.
 * \param node[in] Node name
 * \return true if node name is allowed, false otherwise.
 */
bool spicecompat::check_nodename(QString &node)
{
    static QStringList nutmeg_keywords = {"gt","lt","ge","ne","le","and","not","or","eq"};
    // logical operations (case sensitive)
    //nutmeg_keywords<<"gt"<<"lt"<<"ge"<<"ne"<<"le"<<"and"<<"not"<<"or"<<"eq";
    return !nutmeg_keywords.contains(node);
}

QString spicecompat::getDefaultSimulatorName(int simulator)
{
    QString result;
    switch (simulator) {
        case spicecompat::simNotSpecified:
            result = QObject::tr("Not Specified");
            break;
        case spicecompat::simQucsator:
            result = QObject::tr("Qucsator");
            break;
        case spicecompat::simNgspice:
            result = QObject::tr("Ngspice");
            break;
        case spicecompat::simSpiceOpus:
            result = QObject::tr("SpiceOpus");
            break;
        case spicecompat::simXyce:
            result = QObject::tr("Xyce");
            break;
        default:
            result = QObject::tr("Qucsator");
            break;
    }
    return result;
}
