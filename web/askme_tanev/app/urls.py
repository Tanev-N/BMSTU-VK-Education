from django.urls import path

from . import views

urlpatterns = [
    path('', views.index, name='index'),
    path('hot/', views.hot, name='hot'),
    path('question/<int:question_id>', views.question, name='question'),
    path('ask/', views.ask, name='ask'),
    path('settings/', views.settings, name='settings'),
    path('login/', views.log_in, name='login'),
    path('singup/', views.signup, name='signup'),
    path('tag/<int:tag_id>', views.tag, name='tag'),
    path('logout/', views.logout, name='logout'),
    path('like_ajax/<int:item_id>', views.like_ajax, name='like_ajax'),
    path('answer_correct_flag_ajax/<int:item_id>', views.answer_correct_flag_ajax, name='answer_correct_flag_ajax'),
]
