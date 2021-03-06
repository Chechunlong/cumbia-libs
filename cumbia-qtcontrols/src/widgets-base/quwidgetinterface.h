#ifndef QUWIDGETINTERFACE_H
#define QUWIDGETINTERFACE_H

class CuData;
class QuWidgetUpdateStrategyI;
class QuWidgetContextMenuStrategyI;

/*! \brief an interface to let subclasses provide an update strategy and a context menu
 *         strategy
 *
 * \ingroup corew
 *
 * An example is given by QuPlotBase, that employs a QuPlotBaseContextMenuStrategy
 * to provide a proper context menu.
 */
class QuWidgetInterface
{
public:

    virtual ~QuWidgetInterface() {}

    virtual void update(const CuData& d) = 0;

    virtual void setUpdateStrategy(QuWidgetUpdateStrategyI *updateStrategy) = 0;

    virtual void setContextMenuStrategy(QuWidgetContextMenuStrategyI *ctx_menu_strategy) = 0;
};

#endif // QUWIDGETINTERFACE_H
