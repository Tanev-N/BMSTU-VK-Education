from django.core.paginator import Paginator
from django.shortcuts import render

# Create your views here.



QUESTIONS = [
    {
        "id": i,
        "title": f"Question {i}",
        "text": f"This is question number {i}"
    } for i in range(100)
]
BEST_USERS = [
    {
        "username": f"User{i}"
    } for i in range(10)
]

ANSWERS = [
    {
        "text": f"Answer {i}"
    } for i in range(5)
]


def index(request):
    page_num = request.GET.get('page', 1)
    paginator = Paginator(QUESTIONS, 5)
    page_obj = paginator.page(page_num)
    return render(request, 'index.html', {"questions": page_obj, "best_users": BEST_USERS})


def hot(request):
    questions = QUESTIONS[:]
    questions.reverse()
    page_num = request.GET.get('page', 1)
    paginator = Paginator(questions, 5)
    page_obj = paginator.page(page_num)
    return render(request, 'hot.html', {"questions": page_obj, "best_users": BEST_USERS})


def question(request, question_id):
    question = QUESTIONS[question_id]
    ans = ANSWERS[:]
    page_num = request.GET.get('page', 1)
    paginator = Paginator(ans, 2)
    page_obj = paginator.page(page_num)
    return render(request, "question_details.html",
                  {"question": question, "best_users": BEST_USERS, "answers": page_obj})


def ask(request):
    return render(request, "ask.html", {"best_users": BEST_USERS})


def settings(request):
    return render(request, "settings.html", {"best_users": BEST_USERS})


def signup(request):
    return render(request, "signup.html", {"best_users": BEST_USERS})


def login(request):
    return render(request, "login.html", {"best_users": BEST_USERS})


def tag(request):
    page_num = request.GET.get('page', 1)
    paginator = Paginator(QUESTIONS, 5)
    page_obj = paginator.page(page_num)
    return render(request, 'tag.html', {"questions": page_obj, "best_users": BEST_USERS})
