#include <utility>
#include <algorithm>
#include "../headers/HTMLRepository.h"
#include "../headers/CSVRepository.h"
#include <iostream> //TODO remove this
#if VS == true
#include <windows.h>
#endif

using namespace std;

//
// Created by Horatiu on 20-Mar-19.
//

#include "../headers/Controller.h"

Controller::Controller(Repository &repository) : repository(repository), tutorialIndex(0)
{

}

void Controller::add(const string &title, const string &presenter, Duration duration, int likes, const string &link)
{
    repository.add(new Tutorial(title, presenter, duration, likes, link));
}

const vector<Tutorial *> &Controller::list() const
{
    return repository.getTutorials();
}

void
Controller::update(const string &title, const string &presenter, Duration duration, int likes, const string &link)
{
    repository.update(new Tutorial(title, presenter, duration, likes, link));
}

void Controller::remove(const string &title)
{
    repository.remove(new Tutorial(title));
}


const Tutorial *Controller::next()
{
    const vector<Tutorial *> &tutorials = activeTutorials;
    Tutorial *tutorial = tutorials[tutorialIndex];
    tutorialIndex++;
    if (tutorialIndex == tutorials.size())
    {
        tutorialIndex = 0;
    }
    return tutorial;
}

bool Controller::addToWatchList(const Tutorial *tutorial)
{
    return repository.addToWatchList(tutorial);
}

bool Controller::begin(const string &presenter)
{
    const vector<Tutorial *> &tutorials = repository.getTutorials();
    if (presenter.empty())
    {
        activeTutorials = tutorials;
    }
    else
    {
        activeTutorials = vector<Tutorial *>{};
        copy_if(tutorials.begin(), tutorials.end(), back_inserter(activeTutorials),
                [presenter](const Tutorial *tutorial)
                { return tutorial->getPresenter() == presenter; }
        );
    }

    if (activeTutorials.empty())
    {
        return false;
    }
    tutorialIndex = 0;
    return true;
}

Tutorial *Controller::removeFromWatchList(const string &title)
{
    return repository.removeFromWatchList(title);
}

vector<Tutorial *> Controller::watchList()
{
    return repository.getWatchlist();
}

void Controller::show(const Tutorial *tutorial)
{
//    cout << "Ajunge";
#if VS == true
    ShellExecuteA(nullptr, nullptr, R"("C:\\Program Files (x86)\\Google\\Chrome\\Application\\chrome.exe")", tutorial->getLink().c_str(),
                  nullptr, SW_SHOWMAXIMIZED);
#endif
}

void Controller::showWatchList()
{
    HTMLRepository *htmlRepository = dynamic_cast<HTMLRepository *>(&repository);
    if (htmlRepository != nullptr)
    {
        //TODO fix this
//        std::cout << htmlRepository->watchListDirectory + htmlRepository->watchListPath;
#if VS == true
        ShellExecuteA(nullptr, nullptr, R"("C:\Program Files (x86)\Google\Chrome\Application\chrome.exe")",
                      ("\"" + htmlRepository->watchListDirectory + htmlRepository->watchListPath + "\"").c_str(),
                      nullptr, SW_SHOWMAXIMIZED);
#endif
        return;
    }
    CSVRepository *csvRepository = dynamic_cast<CSVRepository *>(&repository);
    if (csvRepository != nullptr)
    {
#if VS == true
        string command = "notepad \"" + csvRepository->watchlistFile + "\"";
        system(command.c_str());
#endif
        return;
    }
}
