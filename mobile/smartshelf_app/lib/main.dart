import 'dart:async';
import 'dart:ffi';

import 'package:firebase_database/firebase_database.dart';
import 'package:flutter/material.dart';
import 'package:firebase_core/firebase_core.dart';
import 'firebase_options.dart';
import 'package:firebase_messaging/firebase_messaging.dart';
import 'package:flutter_background_service_android/flutter_background_service_android.dart';
import 'package:flutter_local_notifications/flutter_local_notifications.dart';

FirebaseMessaging messaging = FirebaseMessaging.instance;

void main() async {
  WidgetsFlutterBinding.ensureInitialized();
  await Firebase.initializeApp(
    options: DefaultFirebaseOptions.currentPlatform,
  );
  runApp(const SmartShelfApp());
}

class SmartShelfApp extends StatefulWidget {
  const SmartShelfApp({super.key});

  @override
  State<StatefulWidget> createState() => _MyAppState();
}

class _MyAppState extends State<SmartShelfApp> {
  late Future<List<SensorReading>> readings;

  @override
  void initState() {
    readings = getReadings();
    super.initState();
  }

  @override
  Widget build(BuildContext context) {
    setState(() {
      readings = getReadings();
    });

    return MaterialApp(
      home: Scaffold(
        appBar: AppBar(title: const Text("Smart Shelf")),
        body: Center(
          child: FutureBuilder<List<SensorReading>>(
              future: getReadings(),
              builder: (context, snapshot) {
                if (snapshot.hasData) {
                  return ListView.builder(
                      itemCount: snapshot.data!.length,
                      itemBuilder: (context, index) {
                        return ListTile(
                          title: Text(snapshot.data![index].name),
                          subtitle: Text(snapshot.data![index].empty
                              ? "This is empty"
                              : "This is full"),
                        );
                      });
                } else if (snapshot.hasError) {
                  return Text(snapshot.error.toString());
                }
                return CircularProgressIndicator();
              }),
        ),
      ),
    );
  }
}

Future<List<SensorReading>> getReadings() async {
  DatabaseReference starCountRef = FirebaseDatabase.instance.ref();

  List<SensorReading> readings = [];

  starCountRef.onValue.listen((DatabaseEvent event) {
    final data = event.snapshot.value as Map<dynamic, dynamic>;
    data.forEach((key, value) {
      final dataReading = SensorReading.fromMap(value);

      readings.add(dataReading);

      print('${dataReading.name}: ${dataReading.timeEmpty}');
    });
  });

  return readings;
}

class SensorReading {
  final String name;
  final bool empty;
  final int timeEmpty;

  const SensorReading(
      {required this.name, required this.empty, required this.timeEmpty});

  factory SensorReading.fromMap(Map<dynamic, dynamic> map) {
    return SensorReading(
        name: map['Name'],
        empty: map['Empty'] == 0 ? false : true,
        timeEmpty: map['TimeEmpty']);
  }
}
