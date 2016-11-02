#pragma once


class QuaternionManager : public QtVariantPropertyManager
{
	Q_OBJECT
public:
	QuaternionManager(QObject *parent = 0);
	~QuaternionManager();

	virtual QVariant value(const QtProperty *property) const;
	virtual int valueType(int propertyType) const;
	virtual bool isPropertyTypeSupported(int propertyType) const;

	QString valueText(const QtProperty *property) const;

	public Q_SLOTS:
		virtual void setValue(QtProperty *property, const QVariant &val);
protected:
	virtual void initializeProperty(QtProperty *property);
	virtual void uninitializeProperty(QtProperty *property);
	private Q_SLOTS:
		void slotValueChanged(QtProperty *property, const QVariant &value);
		void slotPropertyDestroyed(QtProperty *property);
private:
	struct Data {
		QVariant value;
		QtVariantProperty *w;
		QtVariantProperty *x;
		QtVariantProperty *y;
		QtVariantProperty *z;
	};
	QMap<const QtProperty *, Data> propertyToData;
	QMap<const QtProperty *, QtProperty *> wToProperty;
	QMap<const QtProperty *, QtProperty *> xToProperty;
	QMap<const QtProperty *, QtProperty *> yToProperty;
	QMap<const QtProperty *, QtProperty *> zToProperty;
};

class Vector4Manager : public QtVariantPropertyManager
{
	Q_OBJECT
public:
	Vector4Manager(QObject *parent = 0);
	~Vector4Manager();

	virtual QVariant value(const QtProperty *property) const;
	virtual int valueType(int propertyType) const;
	virtual bool isPropertyTypeSupported(int propertyType) const;

	QString valueText(const QtProperty *property) const;

	void setPropertyNames(QtProperty *property, QString fx, QString fy, QString fz, QString fw);

	public Q_SLOTS:
		virtual void setValue(QtProperty *property, const QVariant &val);
protected:
	virtual void initializeProperty(QtProperty *property);
	virtual void uninitializeProperty(QtProperty *property);
	private Q_SLOTS:
		void slotValueChanged(QtProperty *property, const QVariant &value);
		void slotPropertyDestroyed(QtProperty *property);
private:
	struct Data {
		QVariant value;
		QtVariantProperty *w;
		QtVariantProperty *x;
		QtVariantProperty *y;
		QtVariantProperty *z;
	};
	QMap<const QtProperty *, Data> propertyToData;
	QMap<const QtProperty *, QtProperty *> wToProperty;
	QMap<const QtProperty *, QtProperty *> xToProperty;
	QMap<const QtProperty *, QtProperty *> yToProperty;
	QMap<const QtProperty *, QtProperty *> zToProperty;
};

class Vector3Manager : public QtVariantPropertyManager
{
	Q_OBJECT
public:
	Vector3Manager(QObject *parent = 0);
	~Vector3Manager();

	virtual QVariant value(const QtProperty *property) const;
	virtual int valueType(int propertyType) const;
	virtual bool isPropertyTypeSupported(int propertyType) const;

	QString valueText(const QtProperty *property) const;

	void setPropertyNames(QtProperty *property, QString fx, QString fy, QString fz);

	public Q_SLOTS:
		virtual void setValue(QtProperty *property, const QVariant &val);
protected:
	virtual void initializeProperty(QtProperty *property);
	virtual void uninitializeProperty(QtProperty *property);
	private Q_SLOTS:
		void slotValueChanged(QtProperty *property, const QVariant &value);
		void slotPropertyDestroyed(QtProperty *property);
private:
	struct Data {
		QVariant value;
		QtVariantProperty *x;
		QtVariantProperty *y;
		QtVariantProperty *z;
	};
	QMap<const QtProperty *, Data> propertyToData;
	QMap<const QtProperty *, QtProperty *> xToProperty;
	QMap<const QtProperty *, QtProperty *> yToProperty;
	QMap<const QtProperty *, QtProperty *> zToProperty;
};

class Vector2Manager : public QtVariantPropertyManager
{
	Q_OBJECT
public:
	Vector2Manager(QObject *parent = 0);
	~Vector2Manager();

	virtual QVariant value(const QtProperty *property) const;
	virtual int valueType(int propertyType) const;
	virtual bool isPropertyTypeSupported(int propertyType) const;

	QString valueText(const QtProperty *property) const;

	void setPropertyNames(QtProperty *property, QString fx, QString fy);

	public Q_SLOTS:
		virtual void setValue(QtProperty *property, const QVariant &val);
protected:
	virtual void initializeProperty(QtProperty *property);
	virtual void uninitializeProperty(QtProperty *property);
	private Q_SLOTS:
		void slotValueChanged(QtProperty *property, const QVariant &value);
		void slotPropertyDestroyed(QtProperty *property);
private:
	struct Data {
		QVariant value;
		QtVariantProperty *x;
		QtVariantProperty *y;
	};
	QMap<const QtProperty *, Data> propertyToData;
	QMap<const QtProperty *, QtProperty *> xToProperty;
	QMap<const QtProperty *, QtProperty *> yToProperty;
};


