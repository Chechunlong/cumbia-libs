#ifndef CUTDBPROPERTYREADER_H
#define CUTDBPROPERTYREADER_H

#include <cuthreadlistener.h>
#include <vector>

class CuTDbPropertyReaderPrivate;
class CumbiaTango;
class CuDataListener;


/** \brief Read Tango device, attribute and class properties.
 *
 * CuTDbPropertyReader fetches properties from the Tango database and
 * provides a notification through the CuThreadListener interface when
 * data is available. Data is retrieved in a separate thread.
 *
 * @see CuTDbPropertyReader
 * @see get
 */
class CuTDbPropertyReader : public CuThreadListener
{
public:
    CuTDbPropertyReader(const std::string& id, CumbiaTango *cumbia_t);

    virtual ~CuTDbPropertyReader();

    void get(const std::vector<CuData>& in_data);

    void addListener(CuDataListener *l);

    // CuThreadListener interface
public:
    void onProgress(int step, int total, const CuData &data);
    void onResult(const CuData &data);
    CuData getToken() const;


private:
    CuTDbPropertyReaderPrivate *d;

    // CuThreadListener interface
public:
    void onResult(const std::vector<CuData> &datalist);
};

#endif // CUTDBPROPERTYREADER_H
