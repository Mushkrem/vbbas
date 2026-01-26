#include "csyntaxhighlighter.h"

CSyntaxHighlighter::CSyntaxHighlighter(QTextDocument *parent)
    : QSyntaxHighlighter(parent)
{
    HighlightingRule rule;

    keywordFormat.setForeground(QColor(197, 121, 145));
    keywordFormat.setFontWeight(QFont::Bold);
    const QString keywordPatterns[] = {
        "\\bif\\b", "\\belse\\b", "\\bwhile\\b", "\\bfor\\b",
        "\\bdo\\b", "\\bswitch\\b", "\\bcase\\b", "\\bdefault\\b",
        "\\breturn\\b", "\\bbreak\\b", "\\bcontinue\\b", "\\bgoto\\b",
        "\\bsizeof\\b", "\\btypedef\\b", "\\bstruct\\b", "\\bunion\\b",
        "\\benum\\b", "\\bextern\\b", "\\bstatic\\b", "\\bconst\\b",
        "\\bvolatile\\b", "\\bregister\\b", "\\binline\\b"
    };
    for (const QString &pattern : keywordPatterns) {
        rule.pattern = QRegularExpression(pattern);
        rule.format = keywordFormat;
        highlightingRules.append(rule);
    }

    // Types
    typeFormat.setForeground(QColor(31, 82, 179));
    const QString typePatterns[] = {
        "\\bint\\b", "\\bchar\\b", "\\bfloat\\b", "\\bdouble\\b",
        "\\bvoid\\b", "\\blong\\b", "\\bshort\\b", "\\bunsigned\\b",
        "\\bsigned\\b", "\\bbool\\b", "\\bsize_t\\b",
        "\\bint8_t\\b", "\\bint16_t\\b", "\\bint32_t\\b", "\\bint64_t\\b",
        "\\buint8_t\\b", "\\buint16_t\\b", "\\buint32_t\\b", "\\buint64_t\\b"
    };
    for (const QString &pattern : typePatterns) {
        rule.pattern = QRegularExpression(pattern);
        rule.format = typeFormat;
        highlightingRules.append(rule);
    }

    // Numbers
    numberFormat.setForeground(QColor(80, 175, 90));
    rule.pattern = QRegularExpression("\\b[0-9]+\\.?[0-9]*[fFlLuU]*\\b");
    rule.format = numberFormat;
    highlightingRules.append(rule);

    // Hex numbers
    rule.pattern = QRegularExpression("\\b0[xX][0-9a-fA-F]+[uUlL]*\\b");
    rule.format = numberFormat;
    highlightingRules.append(rule);

    // String
    stringFormat.setForeground(QColor(206, 116, 62));
    rule.pattern = QRegularExpression("\"(?:[^\"\\\\]|\\\\.)*\"");
    rule.format = stringFormat;
    highlightingRules.append(rule);

    // Characters
    rule.pattern = QRegularExpression("'(?:[^'\\\\]|\\\\.)*'");
    rule.format = stringFormat;
    highlightingRules.append(rule);

    // Comments
    singleLineCommentFormat.setForeground(QColor(106, 122, 48));
    rule.pattern = QRegularExpression("//[^\n]*");
    rule.format = singleLineCommentFormat;
    highlightingRules.append(rule);

    // Multi-line comments
    multiLineCommentFormat.setForeground(QColor(106, 122, 48));
    commentStartExpression = QRegularExpression("/\\*");
    commentEndExpression = QRegularExpression("\\*/");
}

void CSyntaxHighlighter::highlightBlock(const QString &text)
{
    for (const HighlightingRule &rule : std::as_const(highlightingRules)) {
        QRegularExpressionMatchIterator matchIterator = rule.pattern.globalMatch(text);
        while (matchIterator.hasNext()) {
            QRegularExpressionMatch match = matchIterator.next();
            setFormat(match.capturedStart(), match.capturedLength(), rule.format);
        }
    }

    setCurrentBlockState(0);

    int startIndex = 0;
    if (previousBlockState() != 1)
        startIndex = text.indexOf(commentStartExpression);

    while (startIndex >= 0) {
        QRegularExpressionMatch match = commentEndExpression.match(text, startIndex);
        int endIndex = match.capturedStart();
        int commentLength;

        if (endIndex == -1) {
            setCurrentBlockState(1);
            commentLength = text.length() - startIndex;
        } else {
            commentLength = endIndex - startIndex + match.capturedLength();
        }

        setFormat(startIndex, commentLength, multiLineCommentFormat);
        startIndex = text.indexOf(commentStartExpression, startIndex + commentLength);
    }
}
