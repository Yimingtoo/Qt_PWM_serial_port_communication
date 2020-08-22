#ifndef HOT_PLUG_H
#define HOT_PLUG_H
#include "mainwindow.h"
#include <QAbstractNativeEventFilter>
#include "windows.h"

#include <dbt.h>

class hot_plug:public QAbstractNativeEventFilter
{



public:
    bool nativeEventFilter(const QByteArray &eventType, void *message, long *result);

signals:
    void hotplug();

};

bool hot_plug::nativeEventFilter(const QByteArray & eventType, void * message, long * result)
{
    MSG* msg = reinterpret_cast<MSG*>(message);
    int msgType = msg->message;
    if (msgType == WM_DEVICECHANGE)
    {
        PDEV_BROADCAST_HDR lpdb = (PDEV_BROADCAST_HDR)msg->lParam;
//		switch (msg->wParam) {
//		case DBT_DEVICEARRIVAL:
//			if (lpdb->dbch_devicetype == DBT_DEVTYP_PORT) {
//				PDEV_BROADCAST_PORT lpdbv = (PDEV_BROADCAST_PORT)lpdb;
//				QString port = QString::fromWCharArray(lpdbv->dbcp_name);//插入的串口名
//				//emit open(port);//发送串口打开信号
//			}
//			break;
//		case DBT_DEVICEREMOVECOMPLETE:
//			if (lpdb->dbch_devicetype == DBT_DEVTYP_PORT) {
//				PDEV_BROADCAST_PORT lpdbv = (PDEV_BROADCAST_PORT)lpdb;
//				QString port = QString::fromWCharArray(lpdbv->dbcp_name);//拔出的串口名
//				//emit close(port);//发送串口关闭信号
//			}
//			break;
//		case DBT_DEVNODES_CHANGED:
//			break;
//		default:
//			break;
//		}
        if(msg->wParam==DBT_DEVICEARRIVAL||msg->wParam==DBT_DEVICEREMOVECOMPLETE)
        {
            if (lpdb->dbch_devicetype == DBT_DEVTYP_PORT)
            {
//                PDEV_BROADCAST_PORT lpdbv = (PDEV_BROADCAST_PORT)lpdb;
//                QString port = QString::fromWCharArray(lpdbv->dbcp_name);//插入的串口名
                emit hotplug();//发送串口打开信号
                return true;
            }
        }
        return false;
    }
    //return QWidget::nativeEvent(eventType, message, result);
    return false;
}


#endif // HOT_PLUG_H
