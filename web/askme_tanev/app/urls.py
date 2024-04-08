from django.urls import path

from . import views

urlpatterns = [
    path('', views.index, name='index'),
    path('hot/', views.hot, name='hot'),
    path('question/<int:question_id>', views.question, name='question'),
    path('ask/', views.ask, name='ask'),
    path('settings/', views.settings, name='settings'),
    path('login/', views.login, name='login'),
    path('singup/', views.signup, name='signup'),
    path('tag/blablabla', views.tag, name='tag'),
]