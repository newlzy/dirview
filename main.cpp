#include <QApplication>
#include <QCommandLineParser> //处理命令行选项的方法
#include <QCommandLineOption> //定义了一个可能的命令行选项
#include <QFileSystemModel> //为本地文件系统提供了一个数据模型
#include <QFileIconProvider> //提供文件图标
#include <QScreen> //查询屏幕属性
#include <QTreeView> //默认的树视图
#pragma execution_character_set("utf-8")
#include <QDebug>


int main(int argc, char *argv[]){

    QApplication app(argc, argv);

    QCommandLineOption p1("a");
    QCommandLineOption p2("b");
    QCommandLineOption p3("c");

    QCommandLineParser pa;
    pa.addOption(p1);
    pa.addOption(p2); //添加解析项
    pa.addOption(p3);
    pa.process(app); //处理命令行参数

    qDebug()<< pa.isSet(p1);
    qDebug()<< pa.isSet(p2);
    qDebug()<< pa.isSet(p3);
    qDebug()<< pa.addOption(p1);
    qDebug()<< pa.addOption(p2);
    qDebug()<< pa.isSet(p3);
    qDebug()<< pa.isSet(p3);

    //设置程序的版本
    QCoreApplication::setApplicationVersion(QT_VERSION_STR);
    QCommandLineParser parser; //处理命令行选项的方法
    //设置应用程序描述
    parser.setApplicationDescription("本地文件树视图");
    parser.addHelpOption(); //显示应用程序描述
    parser.addVersionOption(); //显示程序版本

    //命令行选项。本地文件系统数据模型。默认目录图标。
    QCommandLineOption dontUseCustomDirectoryIconsOption("c","Set QFileSystemModel::DontUseCustomDirectoryIcons");
    //添加在解析时查找的选项
    parser.addOption(dontUseCustomDirectoryIconsOption);

    //不将文件观察者添加到路径中
    QCommandLineOption dontWatchOption("w","Set QFileSystemModel::DontWatch");
    parser.addOption(dontWatchOption);

    //为应用程序定义一个额外的参数，以便使用帮助文本。directory返回当前在对话框中显示的目录。
    parser.addPositionalArgument("directory","The directory to start in.");
    parser.process(app); //处理命令行参数

    //返回位置参数列表。first返回对列表中第一项的引用
    const QString rootPath = parser.positionalArguments().isEmpty() ? QString() : parser.positionalArguments().first();

    QFileSystemModel model; //本地文件系统数据模型
    model.setRootPath("");
    //检查是否将选项名称传递给应用程序，设置了选项名返回true
    if(parser.isSet(dontUseCustomDirectoryIconsOption)){
        //启用给定选项。使用默认目录图标
        model.setOption(QFileSystemModel::DontUseCustomDirectoryIcons);
    }

    if(parser.isSet(dontWatchOption)){
        //不要将文件观察者添加到路径中。当将模型用于像行编辑完成这样的简单任务时，这减少了开销。因为这项目只做显示
        model.setOption(QFileSystemModel::DontWatchForChanges);
    }


    QTreeView tree; //树视图
    tree.setModel(&model);
    if(!rootPath.isEmpty()){ //不为空时
        //cleanPath返回标准化目录分隔符。
        const QModelIndex rootIndex = model.index(QDir::cleanPath(rootPath));
        if(rootIndex.isValid()) //模型索引有效
            tree.setRootIndex(rootIndex); //将根项设置为给定索引处的项
    }

    tree.setAnimated(false); //是否启用动画
    tree.setIndentation(20); //树视图每一层项的缩进
    tree.setSortingEnabled(true); //启用排序

    //screen小部件所在的屏幕。availableGeometry此属性以像素为单位保存屏幕上可用的几何图形
    const QSize availableSize = tree.screen()->availableGeometry().size();
    tree.resize(availableSize / 3); //小部件大小
    //设置列的宽度。width是小部件的宽度
    tree.setColumnWidth(0,tree.width() / 3);

    tree.setWindowTitle(QObject::tr("本地文件目录")); //窗口标题
    tree.show();



    return app.exec();
}
