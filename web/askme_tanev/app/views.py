from django.contrib import auth
from django.contrib.auth import authenticate, login
from django.contrib.auth.decorators import login_required
from django.contrib.auth.models import User
from django.core.paginator import Paginator
from django.http import JsonResponse
from django.shortcuts import render, get_object_or_404, redirect
from django.urls import reverse
from django.views.decorators.csrf import csrf_protect
from django.views.decorators.http import require_http_methods

from .forms import LoginForm, RegisterForm, QuestionForm, SettingsForm, AnswerForm
from .models import Question, Tag, Profile, Answer, QuestionLike, AnswerLike
import json


# Create your views here.


def get_image_url(self):
    profile = Profile.objects.filter(user=self).first()
    return profile.avatar.url


auth.models.User.add_to_class('get_image_url', get_image_url)


def pagination(QuerySet, request, limit=5):
    page_num = request.GET.get('page', 1)
    paginator = Paginator(QuerySet, limit)
    return paginator.get_page(page_num)


@csrf_protect
def index(request):
    new_questions = Question.objects.by_creation_date()
    best_users = Profile.objects.by_rating()[:10]

    tags = Tag.objects.by_popular()[:10]

    page_obj = pagination(new_questions, request)
    return render(request, 'index.html', {"questions": page_obj,
                                          "best_users": best_users,
                                          "tags": tags
                                          })


@csrf_protect
def hot(request):
    hot_question = Question.objects.by_rating()
    best_users = Profile.objects.by_rating()[:10]
    page_obj = pagination(hot_question, request)
    tags = Tag.objects.by_popular()[:10]
    return render(request, 'hot.html', {"questions": page_obj,
                                        "best_users": best_users,
                                        "tags": tags
                                        })


@csrf_protect
def question(request, question_id):
    if request.method == "GET":
        answer_form = AnswerForm(request.user, question_id)
    if request.method == 'POST':
        answer_form = AnswerForm(request.user, question_id, data=request.POST)
        if answer_form.is_valid():
            answer_form.save()
            return redirect('question', question_id=question_id)

    curr_question = Question.objects.filter(id=question_id).first()

    new_answer = Answer.objects.by_creation_date(question_id)
    page_obj = pagination(new_answer, request)
    best_users = Profile.objects.by_rating()[:10]
    tags = Tag.objects.by_popular()[:10]
    return render(request, "question_details.html",
                  {"question": curr_question,
                   "best_users": best_users,
                   "answers": page_obj,
                   "tags": tags,
                   "form": answer_form})


@login_required(redirect_field_name='next', login_url='login')
@csrf_protect
def ask(request):
    if request.method == 'GET':
        question_form = QuestionForm(user=request.user)
    if request.method == 'POST':
        question_form = QuestionForm(user=request.user, data=request.POST)
        if question_form.is_valid():
            question_ = question_form.save()
            return redirect(reverse("question", args=[question_.id]))

    best_users = Profile.objects.by_rating()[:10]
    tags = Tag.objects.by_popular()[:10]

    return render(request, "ask.html", {"form": question_form,
                                        "best_users": best_users,
                                        "tags": tags
                                        })


@login_required(redirect_field_name='next', login_url='login')
@csrf_protect
def settings(request):
    if request.method == 'GET':
        settings_form = SettingsForm(user=request.user,
                                     initial={'username': request.user.username, 'email': request.user.email,
                                              'nickname': request.user.first_name})
    if request.method == 'POST':
        settings_form = SettingsForm(user=request.user, data=request.POST, files=request.FILES)
        if settings_form.is_valid():
            settings_form.save()

    best_users = Profile.objects.by_rating()[:10]
    tags = Tag.objects.by_popular()[:10]
    return render(request, "settings.html", {"form": settings_form,
                                             "best_users": best_users,
                                             "tags": tags
                                             })


@csrf_protect
def signup(request):
    if request.method == "GET":
        register_form = RegisterForm()
    if request.method == "POST":
        register_form = RegisterForm(data=request.POST, files=request.FILES)
        if register_form.is_valid():
            new_profile = register_form.save()
            login(request, new_profile.user)
            return redirect(reverse('index'))
    best_users = Profile.objects.by_rating()[:10]
    tags = Tag.objects.by_popular()[:10]
    return render(request, "signup.html", {"best_users": best_users,
                                           "tags": tags,
                                           "form": register_form
                                           })


@csrf_protect
def log_in(request):
    next_page = request.GET.get('next', reverse('index'))
    if request.method == "GET":
        login_form = LoginForm()
    if request.method == "POST":
        login_form = LoginForm(data=request.POST)
        if login_form.is_valid():
            user = User.objects.filter(username=login_form.cleaned_data['username']).first()
            login(request, user)
            return redirect(next_page)
    best_users = Profile.objects.by_rating()[:10]
    tags = Tag.objects.by_popular()[:10]
    return render(request, "login.html", {"best_users": best_users,
                                          "tags": tags,
                                          "form": login_form
                                          })


def logout(request):
    auth.logout(request)
    return redirect(reverse('login'))


@csrf_protect
def tag(request, tag_id):
    question_with_tag = Question.objects.for_tag(tag_id)
    page_obj = pagination(question_with_tag, request)

    best_users = Profile.objects.by_rating()[:10]
    tags = Tag.objects.by_popular()[:10]
    return render(request, 'tag.html', {"questions": page_obj,
                                        "best_users": best_users,
                                        "tags": tags,
                                        "curr_tag": Tag.objects.filter(id=tag_id).first()
                                        })


def likes_processing(request, item, item_created, item_like, liked):
    if liked['like'] == 'True':
        if not item_created:
            if not item_like.status:
                item_like.status = True
                item.rating += 2
            else:
                item_like.delete()
                item.rating -= 1
                return "delete"
        else:
            item.rating += 1
    else:
        if not item_created:
            if item_like.status:
                item_like.status = False
                item.rating -= 2
            else:
                item_like.delete()
                item.rating += 1
                return "delete"

        else:
            item_like.status = False
            item.rating -= 1
    return "update"


@require_http_methods(["POST"])
@login_required(redirect_field_name='next', login_url='login')
@csrf_protect
def like_ajax(request, item_id):
    liked = json.loads(request.body)
    print(liked)
    profile = Profile.objects.filter(user=request.user).first()
    if liked['item'] == 'Question':
        item = Question.objects.filter(id=item_id).first()
        if not item:
            return JsonResponse({'error': 'not found'}, status=404)
        item_like, item_like_created = QuestionLike.objects.get_or_create(question=item, profile=profile)
    if liked['item'] == 'Answer':
        item = Answer.objects.filter(id=item_id).first()
        if not item:
            return JsonResponse({'error': 'not found'}, status=404)
        item_like, item_like_created = AnswerLike.objects.get_or_create(answer=item, profile=profile)
    if item_like_created:
        print("create")
    else:
        print("get")
    operation = likes_processing(request, item, item_like_created, item_like, liked)
    if operation == 'update':
        item_like.save()
    item.save()
    return JsonResponse({'likes_count': item.rating})


@require_http_methods(["POST"])
@login_required(redirect_field_name='next', login_url='login')
@csrf_protect
def answer_correct_flag_ajax(request, item_id):
    answer = Answer.objects.filter(id=item_id).first()
    if not answer:
        return JsonResponse({'error': 'not found'}, status=404)
    question_for_answer = answer.question
    if request.user != question_for_answer.profile.user:
        return JsonResponse({'error': 'Permission Denied'}, status=403)
    answer.correct = not answer.correct
    answer.save()
    return JsonResponse({}, status=200)
