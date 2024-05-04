from django.core.paginator import Paginator
from django.shortcuts import render, get_object_or_404

from .models import Question, Tag, Profile, Answer


# Create your views here.



def pagination(QuerySet , page, limit = 5):
    paginator = Paginator(QuerySet, limit)
    return paginator.get_page(page)

def index(request):
    page_num = request.GET.get('page', 1)
    new_questions = Question.objects.by_creation_date()
    best_users = Profile.objects.by_rating()[:10]

    tags = Tag.objects.by_popular()[:10]

    page_obj = pagination(new_questions, page_num)
    return render(request, 'index.html', {"questions": page_obj,
                                          "best_users": best_users,
                                          "tags": tags
                                          })


def hot(request):
    page_num = request.GET.get('page', 1)
    hot_question = Question.objects.by_rating()
    best_users = Profile.objects.by_rating()[:10]
    page_obj = pagination(hot_question, page_num)
    tags = Tag.objects.by_popular()[:10]
    return render(request, 'hot.html', {"questions": page_obj,
                                        "best_users":  best_users,
                                        "tags": tags
                                        })


def question(request, question_id):
    page_num = request.GET.get('page', 1)
    new_answer = Answer.objects.by_creation_date(question_id)
    page_obj = pagination(new_answer, page_num)
    curr_question = Question.objects.filter(id=question_id).first()
    best_users = Profile.objects.by_rating()[:10]
    tags = Tag.objects.by_popular()[:10]
    return render(request, "question_details.html",
                  {"question": curr_question,
                   "best_users": best_users,
                   "answers": page_obj,
                   "tags": tags})


def ask(request):

    best_users = Profile.objects.by_rating()[:10]
    tags = Tag.objects.by_popular()[:10]

    return render(request, "ask.html", {"best_users": best_users,
                                        "tags": tags
                                        })


def settings(request):

    best_users = Profile.objects.by_rating()[:10]
    tags = Tag.objects.by_popular()[:10]
    return render(request, "settings.html", {"best_users": best_users,
                                             "tags": tags
                                             })


def signup(request):

    best_users = Profile.objects.by_rating()[:10]
    tags = Tag.objects.by_popular()[:10]
    return render(request, "signup.html", {"best_users": best_users,
                                           "tags": tags
                                           })


def login(request):

    best_users = Profile.objects.by_rating()[:10]
    tags = Tag.objects.by_popular()[:10]
    return render(request, "login.html", {"best_users": best_users,
                                          "tags": tags
                                          })


def tag(request, tag_id):
    page_num = request.GET.get('page', 1)
    question_with_tag = Question.objects.for_tag(tag_id)
    page_obj = pagination(question_with_tag, page_num)

    best_users = Profile.objects.by_rating()[:10]
    tags = Tag.objects.by_popular()[:10]
    return render(request, 'tag.html', {"questions": page_obj,
                                        "best_users": best_users,
                                        "tags": tags,
                                        "curr_tag": Tag.objects.filter(id=tag_id).first()
                                        })
