# XML ↔ JSON Converter

This project provides a lightweight web service that converts **XML ↔ JSON** using Drogon as the web framework. It supports:

* **XML ➝ JSON conversion**
* **JSON ➝ XML conversion**

It uses:

* **Drogon** → High-performance C++ web server
* **PugiXML** → XML parsing
* **Jsoncons** + **JsonCPP** → JSON parsing and generation
* **XPath** + **JsonPath** → Querying and traversing data structures

---

## 🚀 Running the Converter

1. Make sure you have the `xml_json_converter` binary and `config.json` inside the `release/` folder.
2. Ensure the libraries are available (they’re bundled inside `release/lib/`).
3. Make the runner script executable:

```bash
chmod +x run.sh
```

4. Run the server:

```bash
./run.sh
```

You should see a message indicating that Drogon has started.

---

## ⚙️ Config File (`config.json`)

You can configure the **port** and other settings in `config.json`.

Example:

```json
{ "listeners" : [ 
        { "address" : "0.0.0.0", 
        "port" : 5555 
        } 
    ], 
    "runMode" : "Dev" 
}
```

The server will now run on **[http://localhost:5555](http://localhost:5556)**.

---

## 🔄 Endpoints

### 1. XML ➝ JSON

* **URL**: `http://localhost:5555/convert/tojson`
* **Method**: POST
* **Content-Type**: `application/xml`

**Sample XML Input:**

```xml
<Student id="1234">  
    <fname>Raddames</fname>
    <lname>Tonui</lname>
    <Age>25</Age>
    <DOB>21-December-1999</DOB>
    <Profession>
        <title>Software Engineer</title>
        <experience>3 years</experience>
    </Profession>
</Student>
```

**Curl Example:**

```bash
curl -X POST http://localhost:5555/convert/tojson \
     -H "Content-Type: application/xml" \
     -d @student.xml
```

---

### 2. JSON ➝ XML

* **URL**: `http://localhost:5555/convert/toxml`
* **Method**: POST
* **Content-Type**: `application/json`

**Sample JSON Input:**

```json
{
    "Student": {
        "Profession": {
            "experience": "3 years",
            "title": "Software Engineer"
        },
        "age_dob": {
            "age": "25",
            "dob": "21-December-1999"
        },
        "fullname": "Raddames Tonui",
        "student_id": "1234"
    }
}
```

**Curl Example:**

```bash
curl -X POST http://localhost:5555/convert/toxml \
     -H "Content-Type: application/json" \
     -d @student.json
```

---

## ✅ Summary

* Edit `config.json` to change the server port.
* Start the service with `./run.sh`.
* Use the provided endpoints to convert between **XML and JSON** seamlessly.
* System leverages **XPath** and **JsonPath** for traversal, ensuring flexible queries.


[Source code](https://github.com/Raddames-Tonui/xml_json_converter)
