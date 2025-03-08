#ifndef OVERRIDES_H
#define OVERRIDES_H

#include <QHash>
#include <QString>
#include <QRegExp>

// parse to a valid key/value pair
QString overrideKey(const QString &rawKey);

// parse to a valid gamename
QString overrideGamename(const QString &gamenameIn);

// record of gamenames that need to be overridden
const QHash<QString, QString> _validGamenames
{
    {"JetFighter IV", "jetfighter4"},
    {"igi2"         , "projectigi2r"},
};

// record of query keys that need to be overridden
const QHash<QString, QString> _validKeys
{
    {"version",    "gamever"},
    {"mingamever", "minnetver"},
    {"admin",      "adminname"},
    {"adminname",  "adminname"},
    {"admin e-mail",   "adminemail"},
    {"friendly fire",  "friendlyfire"},
    {"friendly fire?", "friendlyfire"},
    {"mapfilename",    "mapname"},
    {"mapid",          "mapname"},
    {"skill",      "botskill"},
    {"num teams",  "maxteams"},
    {"time limit", "timelimit"},
    {"time_limit", "timelimit"},
    {"timetowin",  "timelimit"},
    {"roundtime",  "timelimit"},
    {"active_mods",    "mutators"},
    {"activemod",      "mutators"},
    {"Mutator",        "mutators"},
    {"Mutator101",     "mutators"},
    {"Mutator102",     "mutators"},
    {"Mutator103",     "mutators"},
    {"Mutator104",     "mutators"},
    {"playername",    "player"},
    {"teamname",      "team"},
    {"score",         "score"},
    {"kills",         "score"},
};

// some games do not (fully) support secure/validate
const QStringList _overrideValidateBeacon
{
    "deusex",
    "rune",
    "wot",
    "undying",
    "mobileforces",
};

const QStringList _overrideValidateClient
{
  "deusex",
  "wot",
  "undying",
  "mobileforces",
};

// matching regexes
// TODO: replace with QRegularExpression
const QRegExp _index_match{"^(\\w+)(_\\d+)$"};

#endif // OVERRIDES_H
