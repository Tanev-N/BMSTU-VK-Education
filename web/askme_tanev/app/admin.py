from django.contrib import admin

from app.models import Answer, Question, Profile, AnswerLike, QuestionLike, Tag

# Register your models here.


admin.site.register(Answer)
admin.site.register(Question)
admin.site.register(Profile)
admin.site.register(AnswerLike)
admin.site.register(QuestionLike)
admin.site.register(Tag)
