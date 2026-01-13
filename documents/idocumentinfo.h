#ifndef IDOCUMENTINFO_H
#define IDOCUMENTINFO_H

#include <QString>

class IDocumentInfo
{
public:
    virtual ~IDocumentInfo() = default;
    virtual QString currentDocumentName() const = 0;
    virtual bool currentDocumentModified() const = 0;
    virtual bool hasOpenDocuments() const = 0;
    virtual bool hasAnythingSelected() const = 0;
    virtual int documentCount() const = 0;
};

#endif // IDOCUMENTINFO_H
