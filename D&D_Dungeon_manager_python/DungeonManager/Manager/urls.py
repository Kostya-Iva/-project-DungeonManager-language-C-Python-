from django.urls import path
from .import views

urlpatterns = [
    
    path("", views.home, name="home"),
    path("sheets/", views.my_sheets, name="my_sheets"),
    path("spells/", views.my_spells, name="my_spells"),
    path("General_abilities/", views.my_General_abilities, name="General_abilities"),
    path("Class_abilities/", views.Class_abilities, name="Class_abilities"),
    path("Race_abilities/", views.Race_abilities, name="Race_abilities"),
    ]