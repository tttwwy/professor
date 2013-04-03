
#include <QtGui>
class MyListView : public QListWidget
{
    Q_OBJECT    //自定义信号槽时需加上该宏，否则链接信号槽将会失败


public:
    /*
        columnlabels  QCheckBox名称
        data          QListWidgetItem的用户自定义数据
        cmb           添加列表项的QComboBox,此处主要用于及时的将用户选择的结果显示出来
    */
    MyListView(QWidget* parent=0, QStringList *columnlabels=NULL, QList<QVariant> *data=NULL, QComboBox* cmb=NULL)
    {
        setViewMode(QListWidget::ListMode);
        setSelectionMode(QAbstractItemView::SingleSelection);
        m_cmbox = cmb;

        for( int i=0; i<columnlabels->size(); i++ )
        {
            m_bchecked.append(false);
            m_data.append(data->at(i));

            QListWidgetItem *item = new QListWidgetItem();
            item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable |Qt::ItemIsEnabled);
            item->setData(Qt::UserRole+1, data->at(i));
            insertItem(model()->rowCount(), item);

            QCheckBox* box = new QCheckBox(tr(columnlabels->at(i).toAscii().data()));
            setItemWidget(item,  box);
            m_checklist.append(box);
            m_fslist.append(columnlabels->at(i));
            //链接复选状态改变信号槽

            int val = QObject::connect(box, SIGNAL(stateChanged(int)), this, SLOT(set_select_item(int)));
            qDebug() << tr("链接checkbox消息：%1").arg(val);
        }
    }


public:
    //返回用户选择项的用户自定义数据
    void get_select_data(QList<QVariant> &data)
    {
        for( int i=0; i<m_bchecked.size(); i++)
        {
            if( m_bchecked.at(i)== true)
                data.append( m_data.at(i) );
        }
    }

private slots:
    //QCheckBox复选消息处理
    void set_select_item(int state)
    {
        QCheckBox*  box = qobject_cast<QCheckBox*>(sender());

        for( int off = 0;off<m_checklist.size(); off++)
        {
            if( box == m_checklist.at(off) )
            {
                m_bchecked[off] = (box->checkState() ==  Qt::Checked) ? true : false;
                break;
            }
        }

        QString strfs;
        for( int i=0; i<m_bchecked.size(); i++)
        {
            if( m_bchecked.at(i)== true)
            {
                strfs += m_fslist.at(i);
                strfs += ",";
            }
        }

        qDebug() << tr(strfs.toAscii().data());
        m_cmbox->setEditText( strfs );
    }


private:
    QComboBox         *m_cmbox;
    QList<bool>       m_bchecked;
    QList<QVariant>   m_data;
    QList<QCheckBox*> m_checklist;
    QList<QString>    m_fslist;

};

