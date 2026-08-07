# 🏆 SISTEMA DE GESTIÓN DE TORNEOS - VERSIÓN 3

**Universidad Rafael Urdaneta (URU)**  
**Facultad de Ingeniería - Escuela de Ingeniería de Computación**  
**Asignatura:** Programación 2  
**Profesor:** Ing. Victor Kneider  
**Período:** 2026-B  
**Autor:** David Araujo

---

## 📌 DESCRIPCIÓN DEL PROYECTO
Este proyecto representa la tercera fase evolutiva del Sistema de Gestión de Torneos. Se ha refactorizado por completo la arquitectura monolítica original (Proyecto 2) para implementar un diseño modular basado en los principios de la Programación Orientada a Objetos (POO) en C++. 

El sistema ahora cuenta con encapsulamiento estricto, separación de responsabilidades en 4 capas (Presentación, Negocio, Entidades, Persistencia) y un sistema genérico de manejo de archivos binarios utilizando *Templates*.

---

## 📂 ESTRUCTURA DEL PROYECTO (ARQUITECTURA)

El código fuente está distribuido en módulos independientes según su dominio lógico:

```text
ProyectoGestionTorneo/
│
├── datos/                  # Archivos binarios de persistencia (.bin)
├── equipos/                # Entidad Equipo y OperacionesEquipo
├── interfaz/               # Lógica de presentación y menús
├── jugadores/              # Entidad Jugador y OperacionesJugador
├── partidos/               # Entidades Partido, Gol y OperacionesPartido
├── persistencia/           # GestorArchivos.hpp (Motor template genérico)
├── torneo/                 # Entidad Torneo (Singleton lógico)
├── utilidades/             # Clases estáticas de Validaciones y Formatos
│
├── main.cpp                # Punto de entrada minimalista
├── Makefile                # Script de compilación automatizada
└── README_V3.md            # Documentación del sistema