from django import forms
from django.contrib.auth.models import User
from django.http import request

from .models import Question, Tag, Profile, Answer
from askme_tanev.settings import MEDIA_ROOT
import os
import random
from django.core.files import File

class LoginForm(forms.Form):
    username = forms.CharField(widget=forms.TextInput(attrs={'class': 'form-control'}))
    password = forms.CharField(widget=forms.PasswordInput(attrs={'class': 'form-control'}))

    def clean(self):
        cleaned_data = super(LoginForm, self).clean()
        username = cleaned_data.get('username')
        password = cleaned_data.get('password')
        user = User.objects.filter(username=username).first()
        if user == None:
            raise forms.ValidationError('Incorrect Login or Password')
        if not user.check_password(password):
            raise forms.ValidationError('Incorrect Login or Password')


class RegisterForm(forms.Form):
    username = forms.CharField(widget=forms.TextInput(attrs={'class': 'form-control'}))
    email = forms.EmailField(widget=forms.EmailInput(attrs={'class': 'form-control'}))
    nickname = forms.CharField(widget=forms.TextInput(attrs={'class': 'form-control'}))
    password = forms.CharField(widget=forms.PasswordInput(attrs={'class': 'form-control'}))
    curr_password = forms.CharField(widget=forms.PasswordInput(attrs={'class': 'form-control'}))
    image = forms.ImageField(required=False, widget=forms.FileInput(attrs={'class': 'form-control'}))

    def clean(self):
        cleaned_data = super(RegisterForm, self).clean()
        password = cleaned_data.get('password')
        curr_password = cleaned_data.get('curr_password')
        if password != curr_password:
            raise forms.ValidationError('Password and Current Password do not match')
        user_on_name = User.objects.filter(username=cleaned_data.get('username')).first()
        user_on_email = User.objects.filter(email=cleaned_data.get('email')).first()
        if (user_on_name != None) or (user_on_email != None):
            raise forms.ValidationError('Username or Email is already registered')

    def save(self):
        cl_data = self.cleaned_data
        user = User(username=cl_data['username'], first_name=cl_data['nickname'])
        user.set_password(cl_data['password'])
        user_email = cl_data['email']
        user.email = user_email
        user.save()
        if cl_data.get('image'):
            profile = Profile(user=user, avatar=cl_data['image'])
        else:
            avatars_path = MEDIA_ROOT / 'default_avatars'
            image_files = [os.path.join(avatars_path, f) for f in os.listdir(avatars_path) if
                           os.path.isfile(os.path.join(avatars_path, f))]
            random_image_path = random.choice(image_files)
            image_file = open(random_image_path, 'rb')
            avatar_file = File(image_file, name=os.path.basename(random_image_path))
            profile = Profile(user=user, avatar=avatar_file)
        profile.save()
        return profile


class QuestionForm(forms.Form):
    title = forms.CharField(widget=forms.TextInput(attrs={'class': 'form-control'}))
    text = forms.CharField(widget=forms.Textarea(attrs={'class': 'form-control'}))
    tags = forms.CharField(widget=forms.Textarea(attrs={'class': 'form-control'}))

    def __init__(self, user, **kwargs):
        self._user = user
        super(QuestionForm, self).__init__(**kwargs)

    def clean_text(self):
        text = self.cleaned_data['text']
        if not text:
            raise forms.ValidationError('Please enter a valid text')
        return text

    def clean_title(self):
        title = self.cleaned_data['title']
        if not title:
            raise forms.ValidationError('Please enter a valid title')

        return title

    def clean_tags(self):
        tags = self.cleaned_data['tags']
        tags_list = tags.split(',')
        for tag in tags_list:
            tag.strip()
            if not tag:
                raise forms.ValidationError(f'tag: {tag} is not a valid tag')
        return tags

    def save(self):
        self.cleaned_data['profile'] = self._user
        cl_data = self.cleaned_data
        question = Question(title=cl_data['title'], text=cl_data['text'],
                            profile=Profile.objects.filter(user=self._user).first())
        question.save()
        tags_list = cl_data['tags'].split(',')
        for tag in tags_list:
            tag.strip()
            tag_in_db = Tag.objects.filter(name=tag).first()
            if tag_in_db:
                question.tags.add(tag_in_db)
            else:
                new_tag = Tag(name=tag)
                new_tag.save()
                question.tags.add(new_tag)

        return question


class SettingsForm(forms.Form):
    username = forms.CharField(widget=forms.TextInput(attrs={'class': 'form-control'}))
    email = forms.EmailField(widget=forms.EmailInput(attrs={'class': 'form-control'}))
    nickname = forms.CharField(widget=forms.TextInput(attrs={'class': 'form-control'}))
    avatar = forms.ImageField(required=False, widget=forms.FileInput(attrs={'class': 'form-control'}))

    def __init__(self, user, **kwargs):
        self._user = user
        super(SettingsForm, self).__init__(**kwargs)

    def clean(self):
        username = self.cleaned_data.get('username')
        email = self.cleaned_data.get('email')
        user_with_email = User.objects.filter(email=email).first()
        user_with_username = User.objects.filter(username=username).first()
        if (user_with_email != None) or (user_with_username != None):
            if ((user_with_username != None) and username != self._user.username) or (
                    (user_with_email != None) and email != self._user.email):
                raise forms.ValidationError('Username or Email is already registered')

    def save(self):
        user = self._user
        user.username = self.cleaned_data['username']
        user.email = self.cleaned_data['email']
        user.first_name = self.cleaned_data['nickname']
        user.save()
        profile = Profile.objects.filter(user=user).first()
        profile.avatar = self.cleaned_data['avatar']
        profile.save()

        return user


class AnswerForm(forms.Form):
    Write_answer = forms.CharField(widget=forms.Textarea(attrs={'class': 'form-control'}))
    question_id = None

    def __init__(self, user, question_id_, **kwargs):
        self.question_id = question_id_
        self._user = user
        super(AnswerForm, self).__init__(**kwargs)

    def save(self):
        answer_text = self.cleaned_data['Write_answer']
        question = Question.objects.filter(id=self.question_id).first()
        profile = Profile.objects.filter(user=self._user).first()
        answer = Answer(text=answer_text, question=question, profile=profile, correct=False)
        answer.save()
        return answer
