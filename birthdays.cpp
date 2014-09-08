/*
Luke Wegryn
905739534
lwegryn@vt.edu
ECE 3574
Homework 1
*/

#include <QString>
#include <QDebug>
#include <QTextStream>
#include <QDir>
#include <QDate>
#include <QCommandLineOption>
#include <QCommandLineParser>
#include <QCoreApplication>
#include <QList>
#include <QStringList>

QStringList sortListAlphabetically(QStringList stringList)
{
	qSort(stringList.begin(), stringList.end());
	return stringList;

}

QStringList sortListChronologically(QStringList stringList)
{
	QString datesFirst;
	QString namesFirst;
	QStringList datesFirstList;
	QStringList listToReturn;
	QRegExp expression("(\\ |\\t|\\n)");
	QStringList lineList;
	for(int i = 0; i < stringList.size(); i++)
	{
		lineList = stringList[i].split(expression);
		datesFirst = lineList[2] + '\t' + lineList[0] + ' ' + lineList[1] + '\n';
		datesFirstList.append(datesFirst);
	}

	qSort(datesFirstList.begin(), datesFirstList.end());

	for(int i = 0; i < datesFirstList.size(); i++)
	{
		lineList = datesFirstList[i].split(expression);
		namesFirst = lineList[1] + ' ' + lineList[2] + '\t' + lineList[0];
		listToReturn.append(namesFirst);
	}

	return listToReturn;
}

QStringList sortListChronologicallyUpdate(QStringList stringList)
{
	QString datesFirst;
	QString namesFirst;
	QStringList datesFirstList;
	QStringList listToReturn;
	QRegExp expression("(\\t|\\n)");
	QStringList lineList;
	for(int i = 0; i < stringList.size(); i++)
	{
		lineList = stringList[i].split(expression);
		datesFirst = lineList[1] + '\t' + lineList[0] + '\t' + '  ' + lineList[2] + '\n';
		datesFirstList.append(datesFirst);
	}

	qSort(datesFirstList.begin(), datesFirstList.end());

	for(int i = 0; i < datesFirstList.size(); i++)
	{
		lineList = datesFirstList[i].split(expression);
		namesFirst = lineList[1] + '\t' + lineList[0] + '\t' + lineList[2];
		listToReturn.append(namesFirst);
	}

	return listToReturn;
}

void displayDatesWithinRange(int numDays, QFile &file)
{
	int outputCounter = 0;
	QDate dateNow(QDate::currentDate());
	QString todaysDate = dateNow.toString("yyyy-MM-dd");
	file.open(QFile::ReadWrite);
	QTextStream fileStream(&file);
	QStringList stringList;
	while(!fileStream.atEnd())
	{

		QString line = fileStream.readLine();
		QRegExp expression("(\\ |\\t|\\n)");
		QStringList lineList = line.split(expression);
		QDate lineDate = QDate::fromString(lineList[2], "yyyy-MM-dd");
		if(dateNow.daysTo(lineDate) <= numDays && dateNow.daysTo(lineDate) >= 0)
		{
			outputCounter++;
			stringList.append(line);
		}
	}
	stringList = sortListChronologically(stringList);
	if(outputCounter > 0)
	{
			qDebug() << "  " << "Name" << "\t" << "Birthday";
			qDebug() << "  " << "====" << "\t" << "========";
		for(int i = 0; i < stringList.size(); i++)
		{
			qDebug() << stringList[i];
		}
	}
	else{
		QString temp = "No birthdays to display in the next " + QString::number(numDays) + " days.";
		qDebug() << temp;
	}
	file.close();
}
QDate getBirthDateByName(QString name, QFile &file)
{
	file.open(QFile::ReadOnly);
	QTextStream fileStream(&file);
	QDate lineDate;
	while(!fileStream.atEnd())
	{	
		QString line = fileStream.readLine();
		if(line.contains(name))
		{
			QRegExp expression("(\\ |\t|\\n)");
			QStringList lineList = line.split(expression);
			lineDate = QDate::fromString(lineList[2], "yyyy-MM-dd");
		}
	}
	file.close();
	return lineDate;

}
void displayDatesWithinRangeOfPerson(QString name, QFile &file, int numDays)
{
	QDate dateToCompare = getBirthDateByName(name, file);
	if(dateToCompare.isNull())
	{
		qDebug() << "The name you entered could not be found. Please try again.";
	}
	else
	{
		file.open(QFile::ReadWrite);
		QTextStream fileStream(&file);
		qDebug() << "  " << "Name" << "\t" << "Birthday";
		qDebug() << "  " << "====" << "\t" << "========";
		QStringList stringList;
		while(!fileStream.atEnd())
		{
			QString line = fileStream.readLine();
			QRegExp expression("(\\ |\t|\\n)");
			QStringList lineList = line.split(expression);
			QDate lineDate = QDate::fromString(lineList[2], "yyyy-MM-dd");
			if(dateToCompare.daysTo(lineDate) <= numDays && dateToCompare.daysTo(lineDate) >= 0)
			{
				//qDebug() << line;
				stringList.append(line);
			}
		}
		stringList = sortListChronologically(stringList);
		for(int i = 0; i < stringList.size(); i++)
		{
			qDebug() << stringList[i];
		}

		file.close();
	}
}
bool nameDoesExist(QString name, QFile &file)
{
	bool isFound = false;

	//QFile file("../birthday/birthday.dat");
	file.open(QFile::Truncate | QFile::ReadOnly);
	QTextStream in(&file);
	QString fileString = in.readAll();
	if(fileString.contains(name))
	{
		isFound = true;
	}
	file.close();
	return isFound;

}

void updateAll(QFile &file)
{
	QDate dateNow(QDate::currentDate());
	file.open(QFile::ReadWrite);
	QString toWrite;
	QTextStream fileStream(&file);
	QStringList lineList;
	QStringList stringList;
	QStringList timeUntilList;
	int outputCounter = 0;
	while(!fileStream.atEnd())
	{
		bool isUpdated = false;
		QString line = fileStream.readLine();
		QRegExp expression("(\\ |\\t|\\n)");
		lineList = line.split(expression);
		QDate lineDate = QDate::fromString(lineList[2], "yyyy-MM-dd");
		if(dateNow.daysTo(lineDate) < 0 || dateNow.daysTo(lineDate) > 365)
		{
				isUpdated = true;
		}

		if(lineDate.year() != dateNow.year())
		{
			lineDate.setDate(dateNow.year(), lineDate.month(), lineDate.day());
			lineList[2] = lineDate.toString("yyyy-MM-dd");
		}

		if(dateNow.daysTo(lineDate) < 0)
		{
			lineDate = lineDate.addYears(1);
			lineList[2] = lineDate.toString("yyyy-MM-dd");
		}

		if(isUpdated == true)
		{
			int daysUntil = dateNow.daysTo(lineDate);
			int months = daysUntil/30;
			int weeks = (daysUntil % 30) / 7;
			int days = ((daysUntil % 30) % 7);
			QString stringUntil;
			if(months != 0)
			{
				stringUntil.append(QString::number(months) + " months");
			}
			if(weeks != 0)
			{
				if(months != 0)
					stringUntil.append(", ");
				stringUntil.append(QString::number(weeks) + " weeks");
			}
			if(days != 0)
			{
				if(weeks != 0 || months != 0)
					stringUntil.append(", ");
				stringUntil.append(QString::number(days) + " days");
			}
			stringUntil = lineList[0] + " " + lineList[1] + "\t" + lineList[2] + "\t" + stringUntil;
			//stringList.append(lineList[0] + " " + lineList[1] + "\t" + lineList[2] + "\n");
			//timeUntilList.append(stringUntil);
			//qDebug() << stringUntil;
			stringList.append(stringUntil);
			outputCounter++;
		}
		line = lineList[0] + " " + lineList[1] + "\t" + lineList[2];
		toWrite.append(line + '\n');
	}
	stringList = sortListChronologicallyUpdate(stringList);
	if(outputCounter > 0)
	{
		qDebug() << "  " << "Name" << "\t" << "Birthday" << "\t" << "Time Left Until Birthday";
		qDebug() << "  " << "====" << "\t" << "========" << "\t" << "========================";
		for(int i = 0; i < stringList.size(); i++)
			{
				qDebug() << stringList[i];
			}
	}
	else{
		qDebug() << "Nothing to Update";
	}
	file.resize(0);
	fileStream << toWrite;

	file.close();
}

void searchAndShow(QString name, QFile &file)
{
	file.open(QFile::ReadWrite);
	QString toWrite;
	QTextStream fileStream(&file);
	QStringList stringList;
	int outputCounter = 0;
	while(!fileStream.atEnd())
	{
		QString line = fileStream.readLine();
		if(line.contains(name))
		{
			//qDebug() << line;
			outputCounter++;
			stringList.append(line);
		}
	}
	stringList = sortListAlphabetically(stringList);
	if(outputCounter > 0)
	{
		qDebug() << "  " << "Name" << "\t" << "Birthday";
		qDebug() << "  " << "====" << "\t" << "========";
		for(int i = 0; i < stringList.size(); i++)
			{
				qDebug() << stringList[i];
			}
	}
	else
	{
		qDebug() << "Search query not found. Please try something else.";
	}
	file.close();
}

void removeEntry(QString name, QFile &file)
{

	file.open(QFile::ReadWrite);
	QString toWrite;
	QTextStream fileStream(&file);
	while(!fileStream.atEnd())
	{
		QString line = fileStream.readLine();
		if(!line.contains(name))
			toWrite.append(line + '\n');

	}
	file.resize(0);
	fileStream << toWrite;
	file.close();

}

void replaceEntry(QString name, QString date, QFile &file)
{
	file.open(QFile::ReadWrite);
	QString toWrite;
	QTextStream fileStream(&file);
	while(!fileStream.atEnd())
	{
		QString line = fileStream.readLine();
		if(line.contains(name))
			toWrite.append(name + '\t' + date + '\n');
		else if(!line.contains(name))
		{
			toWrite.append(line + '\n');
		}

	}

	file.resize(0);
	fileStream << toWrite;
	file.close();
}

int main(int argc, char * argv[])
{
	int optionFlag = 0;
	QFile dataFile("birthday.dat");//make a text stream to the data file
	QCoreApplication app(argc, argv);
	QList<QString> inputArgs;
	if(app.arguments().size() > 1)
	{	
		int argSize = app.arguments().size();
		for(int i = 0; i < argSize; i++)
		{
			inputArgs.append(app.arguments()[i]);
		}

		if(inputArgs[1] == NULL)
		{
			optionFlag = 0;
		}
		if(inputArgs[1] == "-a")
		{
			optionFlag = 1;
		}

		else if(inputArgs[1] == "-n")
		{
			optionFlag = 2;
		}

		else if(inputArgs[1] == "-d")
		{
			optionFlag = 3;
		}

		else if (inputArgs[1] == "-m")
		{
			optionFlag = 4;
		}

		else if (inputArgs[1] == "-u")
		{
			optionFlag = 5;
		}

		else if(inputArgs[1] == "-h")
		{
			optionFlag = 6;
		}

		else
		{
			searchAndShow(inputArgs[1], dataFile);
			optionFlag = -1;
			//qDebug() << "hello";
		}
	}
		switch(optionFlag){
			case 0:
				{
					displayDatesWithinRange(30, dataFile);
					break;
				}
			case 1: //this is the -a case, this sets the birthday with the given name
				{
					QDate tryDate = QDate::fromString(inputArgs[2], "yyyy-MM-dd");
					if(!tryDate.isValid())
					{
						qDebug() << "You did not enter a valid date. Please try again.";
					}
					else
					{
						if(nameDoesExist(inputArgs[3]+ ' ' + inputArgs[4] + '\t', dataFile))
							{
								//qDebug() << "That name/date was already entered! I added it anyway.";
								replaceEntry(inputArgs[3]+ ' ' + inputArgs[4], inputArgs[2], dataFile);
							}

						else if(dataFile.open(QFile::Append| QFile::Truncate) && inputArgs.size() >= 5)
						{
							QTextStream outToFile(&dataFile);
							outToFile << inputArgs[3] << ' ' << inputArgs[4] << '\t' << inputArgs[2] << '\n';
							dataFile.close();
						}
						else if (inputArgs.size() < 5)
						{
							qDebug() << "You didn't enter enough arguments, try again.";
						}
					}
					break;
				}
			case 2: //displays all birthdays within X days
				{
					displayDatesWithinRange(inputArgs[2].toInt(), dataFile);
					break;
				}
			case 3: //this should delete the line containing the string
				{
					if(inputArgs.size() == 4)
					{
						removeEntry(inputArgs[2] + ' ' + inputArgs[3] + '\t', dataFile);	
					}

					if(inputArgs.size() == 3)
					{
						QDate tryDate = QDate::fromString(inputArgs[2], "yyyy-MM-dd");
						if(!tryDate.isValid())
						{
							qDebug() << "You did not enter a valid date. Please try again.";
						}
						else
						{
							removeEntry(inputArgs[2], dataFile);
						}
					}
					break;
				}
			case 4: //this is the -m case. This should search for birthdays within X days of Z's birthday
				{
					displayDatesWithinRangeOfPerson(inputArgs[2] + ' ' + inputArgs[3] + '\t', dataFile, inputArgs[4].toInt());
					break;
				}
			case 5: //this is the -u case. This should update all of the contacts to this year.
				{
					updateAll(dataFile);
					break;
				}
			case 6:
				{
					qDebug() << "Usage: ./birthdays [-h -u] [-a 'yyyy-MM-dd' 'FirstName LastName']\n[-n '# of days']' [-d 'FirstName LastName'/'yyyy-MM-dd']\n[-m 'FirstName LastName #ofDays']";
				}

		}

}