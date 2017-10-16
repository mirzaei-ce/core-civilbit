// Copyright (c) 2011-2014 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef CIVILBIT_QT_CIVILBITADDRESSVALIDATOR_H
#define CIVILBIT_QT_CIVILBITADDRESSVALIDATOR_H

#include <QValidator>

/** Base58 entry widget validator, checks for valid characters and
 * removes some whitespace.
 */
class CivilbitAddressEntryValidator : public QValidator
{
    Q_OBJECT

public:
    explicit CivilbitAddressEntryValidator(QObject *parent);

    State validate(QString &input, int &pos) const;
};

/** Civilbit address widget validator, checks for a valid civilbit address.
 */
class CivilbitAddressCheckValidator : public QValidator
{
    Q_OBJECT

public:
    explicit CivilbitAddressCheckValidator(QObject *parent);

    State validate(QString &input, int &pos) const;
};

#endif // CIVILBIT_QT_CIVILBITADDRESSVALIDATOR_H
